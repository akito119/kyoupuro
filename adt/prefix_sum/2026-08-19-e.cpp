#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;


lint main(){
    int n, m;
    cin>>n>>m;

    vector<int> a(n);
    rep(i, n){
        cin>>a[i];
    }

    sort(a.begin(), a.end());

    vector<int> pre(n);

    pre[0] = a[0];

    for(int i = 1;i < n + 1; i++){
        pre[i] = pre[i - 1] + a[i - 1];
    }
    int ans = LLONG_MAX;
    rep(i, n){

    }

}
