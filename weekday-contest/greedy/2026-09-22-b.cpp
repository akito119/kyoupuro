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
    int n,x;
    cin>>n>>x;
    vector<pair<int,int>> p(0);

    rep(i, n){
        int a,b;
        cin>>a>>b;
        if( b< 0){
            continue;
        }

        if(a >= x){
            continue;
        }

        p.push_back({a,b});
    }

    sort(p.begin(),p.end());
    reverse(p.begin(),p.end());
    if(p.size() == 0){
        cout<<0<<endl;
        return 0;
    }

    int ans = p[0].second;

    for(int i = 1;i < p.size(); i++){
        if(p[i].first == p[i - 1].first){
            continue;
        }

        ans += p[i].second;
    }

    cout<<ans<<endl;
}