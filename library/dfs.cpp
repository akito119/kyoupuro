#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;
vector<int> ans(0);
vector<int> path(0);
void dfs(int pos,int n, vector<vector<int>>& g, vector<bool>& visited){
    visited[pos] = true;
    if(pos == n-1){
        path = ans;
        return;
    }
    for(int i = 0; i< g[pos].size(); i++){
        int nex = g[pos][i];
        if(!visited[nex]) {
            ans.push_back(nex);
            dfs(nex,n, g, visited);
            ans.pop_back();
        }
    }
    return;
}

lint main(){
    int n,m;
    cin>>n>>m;
    vector<vector<int>> g(n,vector<int> (0));
    rep(i,m){
        int a,b;
        cin>>a>>b;
        a--,b--;
        g[a].push_back(b);
        g[b].push_back(a);
    }
    vector<bool> visited(n,false);
    dfs(0,n,g,visited);
    cout<<1;
    for(auto x:path) cout<<" "<<x+1;
    cout<<endl;
}