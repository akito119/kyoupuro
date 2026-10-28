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
    int n;
    cin>>n;
    vector<int> b(n);
    vector<int> a(n);
    rep(i, n){
        cin>>a[i];
        cin>>b[i];
    }

    
    vector<int> c(n);
    c[0] = a[0] - b[0];

    for(int i = 1;i<n;i++){
        c[i] = a[i] - b[i];
        c[i] += c[i - 1];
    }

    int ans = LLONG_MAX;

    for(int i = 0;i<n;i++){
        ans = min(ans,c[i]);
    }

    ans = ans < 0 ? -ans:0;
    cout<<ans<<endl;
}
