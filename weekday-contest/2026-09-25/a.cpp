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
    int n,q;
    cin>>n>>q;
    vector<int> a(n);
    rep(i, n){
        cin>>a[i];
    }
    vector<int> b(n);
    rep(i, n){
        cin>>b[i];
    }

    rep(i, q){
        int c;
        cin>>c;
        c--;
        if(c == n -1){
            a[c] = 0;
            continue;
        }

        a[c + 1] = a[c + 1] + max((int) 0, a[c] - b[c]);
        a[c] = 0;
    }

    rep(i, n){
        cout<<a[i]<<endl;
    }
}
