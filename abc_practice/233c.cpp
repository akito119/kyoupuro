#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;
int ans = 0;

void dfs(auto& b, int128 c, int index, int n, int x){
    if(index == n){
        if(c ==x){
            ans ++;
        }
        return;
    }

    for(auto y: b[index]){
        dfs(b, c*y, index + 1, n, x);
    }
    return;
}

lint main(){
   int n,x;
   cin>>n>>x;
    vector<vector<int>> b(n,vector<int> (0));
   rep(i, n){
        int l;
        cin>>l;
        rep(j,l){
            int a;
            cin>>a;
            b[i].push_back(a);
        }
   }
   dfs(b,(int128) 1, 0, n, x);
   cout<<ans<<endl;
}