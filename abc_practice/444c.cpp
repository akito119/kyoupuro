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
    vector<int> a(n);
    rep(i,n) cin>>a[i];

    vector<int> ans = {};

    sort(a.begin(),a.end());

    const int m = a[0] + a[n - 1];
    if( n % 2 == 0){
        ans.push_back(m);
        rep(i, n - 1){
            if(a[i] + a[n - 1 - i] != m){
                ans.pop_back();
                break;
            }
        }
    }
    const int l = a[n - 1];
    while(a[a.size() - 1] == l){
        a.pop_back();
    }

    if(a.size() % 2 == 0){
        ans.push_back(l);
        rep(i, a.size()){
            if(a[i] + a[a.size() - 1 - i] != l){
                ans.pop_back();
                break;
            }
        }
    }
    reverse(ans.begin(),ans.end());
    for(auto x:ans){
        cout<<x<<endl;
    }
}
