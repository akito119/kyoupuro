#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double

#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;
int inf = LLONG_MAX;

//考察
//中心を経由するときとしないときで扱いが変わる
//中心を経由するときはsから中心の最小値 + 中心からtの最小値が最短経路
//そうじゃないときは右回りか左回りのmin
//中心軽油はダイクストラで殺せそう
//そうじゃないときどうすればいいんだろうね
//セグ木？
//累積和みたいな事したらO(1)になりそうじゃね。
//それっぽいわね
//一周にかかる時間を持っておけば左または右回転による値mについてmin(かかる時間 - m, m)でいいね。
//右または左回転のどちらか片方は必ず累積和で計算できる。
lint main() {
    int n, Q;
    cin>>n>>Q;
    int sum = 0;
    vector<int> a(n);
    rep(i, n){
        cin>>a[i];
        sum += a[i];
    }
    vector<int> pre = {0};

    rep(i, n){
        pre.push_back(pre[i] + a[i]);
    }
    vector<vector<pair<int,int>>> g(n + 1, vector<pair<int,int>> {});

    rep(i, n){
        int j = (i + 1) % n;
        g[i].push_back({j, a[i]});
        g[j].push_back({i, a[i]});
    }

    rep(i, n){
        int b;
        cin>>b;
        g[n].push_back({i, b});
        g[i].push_back({n, b});
    }

    priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> q;
    vector<bool> def(n + 1);
    vector<int> cur(n + 1);

    int start = n;
    for(int i= 0;i<n + 1;i++){
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

    rep(i, Q){
        int s, t;
        cin>>s>>t;
        s--,t--;
        if(t == n){
            cout<<cur[s]<<endl;
            continue;
        }
        int path = pre[t] - pre[s];

        cout<<min(min(path, sum - path), cur[s] + cur[t])<<endl;
    }
}