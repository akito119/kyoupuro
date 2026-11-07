#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
// 昇順にソート済みのコンテナ用。該当する要素がなければ end() を返す。
//auto it = get_it(配列, int)で使用。
#define ge_it(a, x) ([](auto&& c, auto&& v) { return lower_bound(c.begin(), c.end(), v); }((a), (x))) // x 以上の最初の要素のイテレータ
#define gt_it(a, x) ([](auto&& c, auto&& v) { return upper_bound(c.begin(), c.end(), v); }((a), (x))) // x より大きい最初の要素のイテレータ
#define le_it(a, x) ([](auto&& c, auto&& v) { auto it = upper_bound(c.begin(), c.end(), v); return it == c.begin() ? c.end() : prev(it); }((a), (x))) // x 以下の最後の要素のイテレータ
#define lt_it(a, x) ([](auto&& c, auto&& v) { auto it = lower_bound(c.begin(), c.end(), v); return it == c.begin() ? c.end() : prev(it); }((a), (x))) // x 未満の最後の要素のイテレータ

int prime = 998244353;
double pie = M_PI;
//from my library https://github.com/akito119/kyoupuro
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