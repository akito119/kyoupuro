#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;
int inf = LLONG_MAX;

lint main(){
   int n;
   cin>>n;
   vector<vector<pair<int,int>>> g(n,vector<pair<int,int>> (0));
   for(int i= 0;i<n-1;i++){
      int a,b,c;
      cin>>a>>b>>c;
      a--;
      b--;
      g[a].push_back({b,c});
      g[b].push_back({a,c});
   }

   int q,k;
   cin>>q>>k;
   k--;

   priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> Q;
    vector<bool> def(n);
    vector<int> cur(n);
    for(int i= 0;i<n;i++){
        def[i] = false;
        cur[i] = inf;
    }
    cur[k] = 0;

    Q.push({cur[k],k});

  

    while(!Q.empty()){
        int pos = Q.top().second;
        Q.pop();
        if(def[pos]){
            continue;
        }
        def[pos] = true;

        for(int i= 0;i<g[pos].size();i++){
            int nex = g[pos][i].first;
            int cost = g[pos][i].second;
            if(cur[nex]>cur[pos] + cost){
                cur[nex] = cur[pos] + cost;
                Q.push({cur[nex],nex});
            }
        }
    }


    for(int i= 0;i<q;i++){
      int x,y;
      cin>>x>>y;
      x--,y--;
      cout<<cur[x]+cur[y]<<endl;
    }
}