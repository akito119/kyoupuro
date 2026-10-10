#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;

// from my library https://github.com/akito119/kyoupuro
// composition(f, g) は、g の後に f を適用する作用を返す。
template <typename T, typename F, typename Mapping, typename Composition>
class DualSegtree
{
private:
    int _n;
    int tree_size;
    Mapping mapping;
    Composition composition;
    F action_idn;
    vector<T> values;
    vector<F> lazy;

    void all_apply(int index, const F &action)
    {
        lazy[index] = composition(action, lazy[index]);
    }

    void push(int index)
    {
        all_apply(2 * index, lazy[index]);
        all_apply(2 * index + 1, lazy[index]);
        lazy[index] = action_idn;
    }

    void apply(int index, int node_l, int node_r,
               int query_l, int query_r, const F &action)
    {
        if (query_r <= node_l || node_r <= query_l)
        {
            return;
        }
        if (query_l <= node_l && node_r <= query_r)
        {
            all_apply(index, action);
            return;
        }
        push(index);
        int middle = (node_l + node_r) / 2;
        apply(2 * index, node_l, middle, query_l, query_r, action);
        apply(2 * index + 1, middle, node_r, query_l, query_r, action);
    }

    T value(int index, int node_l, int node_r, int position)
    {
        if (node_r - node_l == 1)
        {
            return mapping(lazy[index], values[position]);
        }
        push(index);
        int middle = (node_l + node_r) / 2;
        if (position < middle)
        {
            return value(2 * index, node_l, middle, position);
        }
        return value(2 * index + 1, middle, node_r, position);
    }

    void insert(int index, int node_l, int node_r,
                int position, const T &new_value)
    {
        if (node_r - node_l == 1)
        {
            values[position] = new_value;
            lazy[index] = action_idn;
            return;
        }
        push(index);
        int middle = (node_l + node_r) / 2;
        if (position < middle)
        {
            insert(2 * index, node_l, middle, position, new_value);
        }
        else
        {
            insert(2 * index + 1, middle, node_r, position, new_value);
        }
    }

public:
    DualSegtree(int n, T initial_value, Mapping mapping,
                Composition composition, F action_idn)
        : DualSegtree(vector<T>(n, initial_value), move(mapping),
                      move(composition), action_idn)
    {
    }

    DualSegtree(const vector<T> &values, Mapping mapping,
                Composition composition, F action_idn)
        : _n(static_cast<int>(values.size())),
          tree_size(1),
          mapping(move(mapping)),
          composition(move(composition)),
          action_idn(move(action_idn)),
          values(values)
    {
        while (tree_size < _n)
        {
            tree_size *= 2;
        }
        lazy.assign(2 * tree_size, this->action_idn);
    }

    // index 番目の値を new_value に変更し、それ以前の作用を取り除く。
    void insert(int index, const T &new_value)
    {
        assert(0 <= index && index < _n);
        insert(1, 0, tree_size, index, new_value);
    }

    // index 番目の値に action を適用する。
    void apply(int index, const F &action)
    {
        assert(0 <= index && index < _n);
        apply(index, index + 1, action);
    }

    // 半開区間 [l, r) に action を適用する。
    void apply(int l, int r, const F &action)
    {
        assert(0 <= l && l <= r && r <= _n);
        if (l == r)
        {
            return;
        }
        apply(1, 0, tree_size, l, r, action);
    }

    // index 番目の作用適用後の値を返す。
    T value(int index)
    {
        assert(0 <= index && index < _n);
        return value(1, 0, tree_size, index);
    }
};

/*
区間加算・一点取得で使う例：

long long mapping(long long f, long long value)
{
    return value + f;
}

long long composition(long long f, long long g)
{
    return f + g;
}

vector<long long> initial(n);
DualSegtree seg(initial, mapping, composition, 0LL);
seg.apply(l, r, x);
cout << seg.value(index) << '\n';
*/
