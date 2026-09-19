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
    int n,k;
    cin>>n>>k;
    vector<int> a(n);
    vector<int> s(n);
    rep(i, n){
        cin>>a[i];
        s[i] = a[i]%k;
    }
    int sum = 0;
    set<int> res = {0};
    int ans = 0;
    rep(i, n){
        sum += s[i];
        sum %= k;
        if(res.count(sum)){
            res = {0};
            sum = 0;
            ans ++;
        }
        else{
            res.insert(sum);
        }
    }
    cout<<ans<<endl;
}
