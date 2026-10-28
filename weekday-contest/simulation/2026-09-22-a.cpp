#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double

#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;
int inf = LLONG_MAX;


lint main() {
    int n,m;
    cin>>n>>m;
    vector<int> s(n);
    rep(i, n){
        cin>>s[i];
    }

    rep(i, m){
        int t,p;
        cin>>t>>p;
        t--;
        if(s[t] >= p){
            s[t] -=p;
        }
    }

    int ans = 0;
    rep(i, n){
        if(s[i] ==0){
            ans ++;
        }
    }
    cout<<ans<<endl;
}