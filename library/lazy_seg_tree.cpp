#include <bits/stdc++.h>
using namespace std;

// from my library https://github.com/akito119/kyoupuro
//
// op(a, b)             : 値モノイドの演算
// mapping(f, x, size)   : 要素数 size の区間値 x に作用 f を適用
//                         区間長が不要なら mapping(f, x) でもよい
// composition(f, g)    : g を適用した後に f を適用する作用
// idn / action_idn     : それぞれのモノイドの単位元
template <typename T, typename F, typename Op, typename Mapping,
          typename Composition>
class LazySegtree
{
private:
    int _n;
    int tree_size;
    int log_size;
    Op op;
    Mapping mapping;
    Composition composition;
    T idn;
    F action_idn;
    vector<T> seg_tree;
    vector<F> lazy;
    vector<int> range_size;

    T apply_mapping(const F &action, const T &value, int size)
    {
        if constexpr (is_invocable_r_v<T, Mapping, F, T, int>)
        {
            return mapping(action, value, size);
        }
        else
        {
            return mapping(action, value);
        }
    }

    void update(int index)
    {
        seg_tree[index] = op(seg_tree[2 * index], seg_tree[2 * index + 1]);
    }

    void all_apply(int index, const F &action)
    {
        seg_tree[index] = apply_mapping(action, seg_tree[index], range_size[index]);
        if (index < tree_size)
        {
            lazy[index] = composition(action, lazy[index]);
        }
    }

    void push(int index)
    {
        all_apply(2 * index, lazy[index]);
        all_apply(2 * index + 1, lazy[index]);
        lazy[index] = action_idn;
    }

public:
    LazySegtree(int n, Op op, T idn, Mapping mapping,
                Composition composition, F action_idn)
        : LazySegtree(vector<T>(n, idn), move(op), idn, move(mapping),
                      move(composition), action_idn)
    {
    }

    LazySegtree(const vector<T> &values, Op op, T idn, Mapping mapping,
                Composition composition, F action_idn)
        : _n(static_cast<int>(values.size())),
          tree_size(1),
          log_size(0),
          op(move(op)),
          mapping(move(mapping)),
          composition(move(composition)),
          idn(move(idn)),
          action_idn(move(action_idn))
    {
        while (tree_size < _n)
        {
            tree_size *= 2;
            ++log_size;
        }
        seg_tree.assign(2 * tree_size, this->idn);
        lazy.assign(tree_size, this->action_idn);
        range_size.assign(2 * tree_size, 0);
        for (int i = 0; i < _n; ++i)
        {
            seg_tree[tree_size + i] = values[i];
            range_size[tree_size + i] = 1;
        }
        for (int i = tree_size - 1; i > 0; --i)
        {
            range_size[i] = range_size[2 * i] + range_size[2 * i + 1];
            update(i);
        }
    }

    // index 番目の値を val に変更する。
    void insert(int index, const T &val)
    {
        assert(0 <= index && index < _n);
        int node = tree_size + index;
        for (int i = log_size; i > 0; --i)
        {
            push(node >> i);
        }
        seg_tree[node] = val;
        for (int i = 1; i <= log_size; ++i)
        {
            update(node >> i);
        }
    }

    // index 番目の値に action を適用する。
    void apply(int index, const F &action)
    {
        assert(0 <= index && index < _n);
        int node = tree_size + index;
        for (int i = log_size; i > 0; --i)
        {
            push(node >> i);
        }
        seg_tree[node] = apply_mapping(action, seg_tree[node], 1);
        for (int i = 1; i <= log_size; ++i)
        {
            update(node >> i);
        }
    }

    // 半開区間 [l, r) に action を適用する。
    void apply(int l, int r, const F &action)
    {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r)
        {
            return;
        }

        l += tree_size;
        r += tree_size;
        const int left = l;
        const int right = r;

        for (int i = log_size; i > 0; --i)
        {
            if (((left >> i) << i) != left)
            {
                push(left >> i);
            }
            if (((right >> i) << i) != right)
            {
                push((right - 1) >> i);
            }
        }

        while (l < r)
        {
            if (l & 1)
            {
                all_apply(l++, action);
            }
            if (r & 1)
            {
                all_apply(--r, action);
            }
            l >>= 1;
            r >>= 1;
        }

        for (int i = 1; i <= log_size; ++i)
        {
            if (((left >> i) << i) != left)
            {
                update(left >> i);
            }
            if (((right >> i) << i) != right)
            {
                update((right - 1) >> i);
            }
        }
    }

    // 半開区間 [l, r) の積を返す。
    T query(int l, int r)
    {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r)
        {
            return idn;
        }

        l += tree_size;
        r += tree_size;
        for (int i = log_size; i > 0; --i)
        {
            if (((l >> i) << i) != l)
            {
                push(l >> i);
            }
            if (((r >> i) << i) != r)
            {
                push((r - 1) >> i);
            }
        }

        T left_result = idn;
        T right_result = idn;
        while (l < r)
        {
            if (l & 1)
            {
                left_result = op(left_result, seg_tree[l++]);
            }
            if (r & 1)
            {
                right_result = op(seg_tree[--r], right_result);
            }
            l >>= 1;
            r >>= 1;
        }
        return op(left_result, right_result);
    }

    T value(int index)
    {
        return query(index, index + 1);
    }

    T all_query() const
    {
        return seg_tree[1];
    }
};

/*
区間加算・区間和で使う例：

auto op = [](long long a, long long b) -> long long {
    return a + b;
};
auto mapping = [](long long f, long long x, int size) -> long long {
    return x + f * size;
};
auto composition = [](long long f, long long g) -> long long {
    return f + g;
};

vector<long long> values(n);
LazySegtree seg(values, op, 0LL, mapping, composition, 0LL);

// 通常の Segtree と同じく、要素数から空の木を作ることもできる。
LazySegtree empty_seg(n, op, 0LL, mapping, composition, 0LL);
*/
