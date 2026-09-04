#include <bits/stdc++.h>
using namespace std;
using lint = int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;

template <typename T, typename Op>
class Segtree
{
private:
    int _n;
    int tree_size;
    Op op;
    T idn;
    vector<T> seg_tree;

    int set_tree_size(int n)
    {
        int size = 1;
        while (size < n)
        {
            size *= 2;
        }
        return size;
    }
    void add(int index, T com)
    {
        seg_tree[index] = com;
        while (index > 0)
        {
            index /= 2;
            seg_tree[index] = op(seg_tree[2 * index], seg_tree[2 * index + 1]);
        }
        return;
    }

    int fix_index(int index)
    {
        return tree_size + index;
    }

public:
    Segtree(int n, Op op, T idn)
        : _n(n),
          tree_size(set_tree_size(n)),
          op(move(op)),
          idn(idn),
          seg_tree(2 * tree_size, idn)
    {
    }

    void insert(int index, T val)
    {
        add(fix_index(index), val);
        return;
    }

    T query(int l, int r)
    {
        int left = fix_index(l);
        int right = fix_index(r);

        T left_result = idn;
        T right_result = idn;

        while (left < right)
        {
            // left が右側の子なら、そのノードを採用
            if (left % 2 == 1)
            {
                left_result = op(left_result, seg_tree[left]);
                ++left;
            }

            // right が右側の子なら、直前のノードを採用
            if (right % 2 == 1)
            {
                --right;
                right_result = op(seg_tree[right], right_result);
            }

            left /= 2;
            right /= 2;
        }

        return op(left_result, right_result);
    }

    T value(int index){
        return seg_tree[fix_index(index)];
    }
};

int op(int a, int b){
    //モノイドの演算を記入する。
    //必要に応じて引数、返り値を変更する。
}
