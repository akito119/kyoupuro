#include <bits/stdc++.h>
using namespace std;
using lint = int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;
//from my library https://github.com/akito119/kyoupuro
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
    return a + b;
}
//添え字の正規化をlogでやりたい問題？
//LとRの差が提示されていない以上愚直に正規化するだけではうまくいかない気がする。
//クエリ処理だからさすがにセグ木ではあるとは思うが。
//例えばL - endとfirst - Rをセグ木処理することで、end - first間はより高速に処理できそうですよね。
//方針決定

//upper_boundかなぁ

lint main() {
    int n, q;
    cin>>n>>q;
    Segtree seg(n, op, (int)0);
    Segtree sum(n, op, (int)0);
    rep(i, n){
        int a;
        cin>>a;
        seg.insert(i, a);
        sum.insert(i, seg.query(0, i + 1));
    }
    vector<int> ind(n);
    ind[0] = 0;
    for(int i = 1;i< n;i++){
        ind[i] = ind[i - 1] + i;
    }

    rep(i, q){
        int l,r;
        cin>>l>>r;
        l--,r--;


        int first,end;
        first = prev(upper_bound(ind.begin(), ind.end(), l)) - ind.begin();
        end  = prev(upper_bound(ind.begin(), ind.end(), r)) - ind.begin();
        //cout<<first<<" "<<end<<endl;
        int ans = 0;
        if(first == end){
            ans += seg.query(l - ind[first], r-ind[end] + 1);
            cout<<ans<<endl;
            continue;
        }
        ans += seg.query(l - ind[first], first + 1);
        //cout<<ans<<endl;
        ans += seg.query(0, r - ind[end] + 1);
        //cout<<ans<<" "<<r-ind[end] + 1<<endl;
        if(end - first > 1){
            ans += sum.query(first + 1, end);
        }
        cout<<ans<<endl;
    }
}
