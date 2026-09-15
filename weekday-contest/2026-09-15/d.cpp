#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;
//from my library https://github.com/akito119/kyoupuro


lint main() {
    int n;
    cin>>n;
    int m;
    cin>>m;
    vector<int> a(n);
    rep(i, n){
        cin>>a[i];
    }

    vector<int> l(m);

    rep(i, m){
        cin>>l[i];
    }
    vector<int> c(n - 1);
    if(m < n - 1){
        cout<<"No"<<endl;
        return 0;
    }
    sort(a.begin(),a.end());

    for(int i = 1;i<n;i++){
        c[i - 1] = a[0] + a[i];
    }

    sort(c.begin(),c.end());
    sort(l.begin(),l.end());
    reverse(c.begin(),c.end());
    reverse(l.begin(),l.end());

    rep(i,n - 1){
        if(c[i] > l[i]){
            cout<<"No"<<endl;
            return 0;
        }
    }
    cout<<"Yes"<<endl;

}