#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
// 昇順にソート済みのコンテナ用。該当する要素がなければ end() を返す。
//auto it = get_it(配列, int)で使用。
#define ge_it(a, x) ([](auto&& c, auto&& v) { return lower_bound(c.begin(), c.end(), v); }((a), (x))) // x 以上の最初の要素のイテレータ
#define gt_it(a, x) ([](auto&& c, auto&& v) { return upper_bound(c.begin(), c.end(), v); }((a), (x))) // x より大きい最初の要素のイテレータ
#define le_it(a, x) ([](auto&& c, auto&& v) { auto it = upper_bound(c.begin(), c.end(), v); return it == c.begin() ? c.end() : prev(it); }((a), (x))) // x 以下の最後の要素のイテレータ
#define lt_it(a, x) ([](auto&& c, auto&& v) { auto it = lower_bound(c.begin(), c.end(), v); return it == c.begin() ? c.end() : prev(it); }((a), (x))) // x 未満の最後の要素のイテレータ

int prime = 998244353;
double pie = M_PI;


lint main(){
 
}
