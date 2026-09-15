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
    set<int> c;
    rep(i,n){
        int t;
        cin>>t;
        c.insert(t);
    }

    cout<<n-c.size()<<endl;
}
