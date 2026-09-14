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
    int n,s,l;
    cin>>n>>s>>l;
    vector<int> a(n - 1);
    rep(i,n - 1) cin>>a[i];
    if(s == 1){
        int ans = 0;
        int num = 0;
        rep(i,n){
            ans +=a[i];
            num ++;
            if( ans > l){
                cout<<num <<endl;
                return 0;
            }
        }
        cout<<n<<endl;
        return 0;
    }
    if(s == n){
        int ans = 0;
        int num = 0;
        for(int i = n - 2;i>=0;i--){
            ans += a[i];
            num++;
            if(ans > l){
                cout<<num <<endl;
                return 0;
            }
        }
        cout<<n<<endl;
        return 0;
    }
    if(l <a[s-1] && l < a[s - 2]){
        cout<<1<<endl;
        return 0;
    }
    vector<pair<int,int>> sl(0),sr(0);
    sl.push_back({0,0});
    int num = 0;
    for(int i = s-2;i>=0;i--){
        num ++;
        sl.push_back({sl[num - 1].first + a[i],sl[num - 1].second + 1});
    }
    sr.push_back({0,0});
    num = 0;
    for(int i = s - 1;i < n-1;i++){
        num ++;
        sr.push_back({sr[num - 1].first + a[i],sr[num - 1].second + 1});
    }
    int ans1 = 0,ans2 = 0;
    for(auto x:sl){
        for(auto y:sr){
            int lc = x.first,rc = y.first,ls = x.second,rs = y.second;

            if(lc * 2 + rc <= l){
                ans1 = max(ans1,ls + rs);
            }
            if(lc + rc * 2 <= l){
                ans2 = max(ans2,ls + rs);
            }
        }
    }
    cout<<max(ans1,ans2) + 1<<endl;
}
