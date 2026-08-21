#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;
int inf = LLONG_MAX;

lint main(){
    int n,m,y;
    cin>>n>>m>>y;
    vector<vector<pair<int,int>>> g(n,vector<pair<int,int>> (0));

    
    for(int i= 0;i<m;i++){
        int u,v,t;
        cin>>u>>v>>t;
        u--,v--;
        g[u].push_back({v,t});
        g[v].push_back({u,t});
    }

    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> q;
    vector<bool> def(n);
    vector<int> cur(n);

    int start = 0;
    for(int i= 0;i<n;i++){
        def[i] = false;
        cur[i] = inf;
    }
    cur[start] = 0;

    q.push({cur[start],start});

  

    while(!q.empty()){
        int pos = q.top().second;
        q.pop();
        if(def[pos]){
            continue;
        }
        def[pos] = true;

        for(int i= 0;i<g[pos].size();i++){
            int nex = g[pos][i].first;
            int cost = g[pos][i].second;
            if(cur[nex]>cur[pos] + cost){
                cur[nex] = cur[pos] + cost;
                q.push({cur[nex],nex});
            }
        }
    }
    for(int i= 1;i<n;i++){
            cout<<cur[i]<<endl;
    }
}