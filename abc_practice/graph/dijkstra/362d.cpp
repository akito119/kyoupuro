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
    int n,m;
    cin>>n>>m;
    vector<vector<pair<int,int>>> load(n,vector<pair<int,int>> (0));
    vector<int> a(n);
    for(int i=0;i<n;i++){
        cin>>a[i];
    }
    for(int i = 0;i<m;i++){
        int u,v,b;
        cin>>u>>v>>b;
        u--,v--;
        load[u].push_back({v,b+a[u]});
        load[v].push_back({u,b+a[v]});
    }
    

    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> q;
    vector<bool> def(n);
    vector<int> ans(n);
    for(int i= 0;i<n;i++){
        def[i] = false;
        ans[i] = inf;
    }
    ans[0] = 0;

    q.push({ans[0],0});

  

    while(!q.empty()){
        int pos = q.top().second;
        q.pop();
        if(def[pos]){
            continue;
        }
        def[pos] = true;

        for(int i= 0;i<load[pos].size();i++){
            int nex = load[pos][i].first;
            int cost = load[pos][i].second;
            if(ans[nex]>ans[pos] + cost){
                ans[nex] = ans[pos] + cost;
                q.push({ans[nex],nex});
            }
        }
    }
    for(int i=1;i<n;i++){
        cout<<ans[i]+a[i]<<endl;
    }
}