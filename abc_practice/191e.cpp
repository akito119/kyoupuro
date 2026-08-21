#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;
int inf = LLONG_MAX;
//ダイクストラを各点でまわすやつ
lint main(){
    int n,m;
    cin>>n>>m;
    vector<vector<pair<int,int>>> g(n,vector<pair<int,int>> (0));

    vector<vector<int>> ans(n,vector<int> (n));
    for(int i= 0;i<n;i++){
        for(int j= 0;j<n;j++){
            ans[i][j] = inf;
        }
    }
    for(int i= 0;i<m;i++){
        int a,b,c;
        cin>>a>>b>>c;
        a--;
        b--;
        g[a].push_back({b,c});
        if(a == b){
            ans[a][b] = min(c,ans[a][b]);
        }
    }
    for(int k = 0;k<n;k++){

    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> q;
    vector<bool> def(n);
    vector<int> cur(n);
    for(int i= 0;i<n;i++){
        def[i] = false;
        cur[i] = inf;
    }
    cur[k] = 0;

    q.push({cur[k],k});

  

    while(!q.empty()){
        int pos = q.top().second;
        q.pop();
        if(pos != k) ans[k][pos] = cur[pos];
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

    for(int i= 0;i<n;i++){
        int a = ans[i][i];
        for(int j = 0;j<n;j++){
            if(ans[i][j] != inf && ans[j][i] != inf && i != j){
                int b = ans[i][j] + ans[j][i];
                a = min(a,b);
            }
        }
        if(a == inf){
            cout<<-1<<endl;
        }
        else{
            cout<<a<<endl;
        }
    }
}
