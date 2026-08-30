#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;

void dfs(vector<int>& ans, int n, int m){
    if(ans.size() == n){
        rep(i,n) {
            if(i == n-1){
                cout<<ans[i]<<endl;
            }
            else{
                cout<<ans[i]<<" ";
            }
        }
        return;
    }

    for(int i = 1;i <= m; i++){
        if(ans.size() >= 1){
            if(ans[ans.size()-1]>= i){
                continue;
            }
        }
        ans.push_back(i);
        dfs(ans, n, m);
        ans.pop_back();
    }
    return;
}

lint main(){
   int n, m;
   cin>>n>>m;
    vector<int> ans = {};
    dfs(ans, n, m);
}