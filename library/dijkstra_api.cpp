#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;
int inf = LLONG_MAX;
//from my library https://github.com/akito119/kyoupuro

void dijkstra(auto& g, auto q,int n, int start, vector<int>& cur, vector<bool>& def){
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
}

lint main(){
    
    
}