#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;
int inf = LLONG_MAX;
//十分に賢ければこの問題はiからjにコストcijのパスが存在するととらえて,ダイクストラと気づくことができる
lint main(){
    int h,w;
    cin>>h>>w;
    vector<vector<pair<int,int>>> g(10,vector<pair<int,int>> (0));
    for(int i= 0;i<10;i++){
        for(int j = 0; j < 10; j ++){
            int c;
            cin>>c;
            g[j].push_back({i,c});
        }
    }
    int n = 10;
    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> q;
    vector<bool> def(n);
    vector<int> cur(n);
    for(int i= 0;i<n;i++){
        def[i] = false;
        cur[i] = inf;
    }
    cur[1] = 0;

    q.push({cur[1],1});

  

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
    int ans = 0;
    for(int i=0 ;i<10;i++){
        //cout<<i<<" "<<cur[i]<<endl;
    }
    for(int i= 0;i<h;i++){
        for(int j = 0;j<w;j++){
            int a;
            cin>>a;
            if(a == -1){
                continue;
            }
            else if(a == 1){
                continue;
            }
            ans += cur[a];
        }
    }
    cout<<ans<<endl;
}
