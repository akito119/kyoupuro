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
    int n;
    cin>>n;
    vector<vector<pair<int,int>>> load(n,vector<pair<int,int>> (0));

    for(int i = 0; i < n-1; i++){
        int a,b,x;
        cin>>a>>b>>x;
        load[i].push_back({i+1,a});
        x--;
        load[i].push_back({x,b});
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
    cout<<ans[n-1]<<endl;
}