#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;
//from my library https://github.com/akito119/kyoupuro
template <class E> struct csr {
    vector<int> start;
    vector<E> elist;
    explicit csr(int n, const vector<pair<int, E>>& edges)
        : start(n + 1), elist(edges.size()) {
            for (auto e : edges) {
                start[e.first + 1]++;
            }
            for (int i = 1; i <= n; i++){
                start[i] += start[i-1];
            }
            auto counter = start;
            for(auto e : edges) {
                elist[counter[e.first]++] = e.second;
            }
        }
};

struct scc_graph{
    private:
        int _n;
        struct edge {
            int to;
        };
        vector<pair<int, edge>> edges;
    public:
        explicit scc_graph(int n) : _n(n) {}
        int num_vertices() {return _n; }
        void add_edge(int from, int to) {edges.push_back({from,{to}});}

        pair<int, vector<int>> scc_ids() {
            auto g = csr<edge>(_n, edges);
            int now_ord = 0, group_num = 0;
            vector<int> visited, low(_n), ord(_n, -1), ids(_n);
            visited.reserve(_n);
            auto dfs = [&](auto self, int v) -> void {
                low[v] = ord[v] = now_ord++;
                visited.push_back(v);
                for(int i = g.start[v]; i < g.start[v + 1]; i++) {
                    auto to = g.elist[i].to;
                    if(ord[to] == -1) {
                        self(self, to);
                        low[v] = min(low[v], low[to]);
                    }
                    else{
                        low[v] = min(low[v], ord[to]);
                    }
                }
                if(low[v] == ord[v]){
                    while(true) {
                        int u = visited.back();
                        visited.pop_back();
                        ord[u] = _n;
                        ids[u] = group_num;
                        if(u == v) break;
                    }
                    group_num ++;
                }
            };
            for ( int i = 0; i < _n; i++){
                if (ord[i] == -1) dfs(dfs, i);
            }
            for(auto& x : ids) {
                x = group_num - 1 - x;
            }
            return {group_num, ids};
        }

        vector<vector<int>> scc() {
            auto ids = scc_ids();
            int group_num = ids.first;
            vector<int> counts(group_num);
            for(auto x : ids.second) counts[x]++;
            vector<vector<int>> groups(ids.first);
            for(int i = 0; i < group_num; i++){
                groups[i].reserve(counts[i]);
            }
            for (int i = 0; i < _n; i++) {
                groups[ids.second[i]].push_back(i);
            }
            return groups;
        }
};


lint main() {
    int n,m;
    cin>>n>>m;
    vector<int> w(n);
    rep(i,n){
        cin>>w[i];
    }

    vector<int> t(n);
    rep(i, n){
        cin>>t[i];
    }

    vector<int> dif(n);
    rep(i, n){
        dif[i] = t[i] - w[i] > 0 ? t[i] - w[i] : 0;
    }

    scc_graph scc(n);

    rep(i,m){
        int u,v;
        cin>>u>>v;
        u--,v--;
        scc.add_edge(u,v);
    }

    auto group = scc.scc();

    vector<int> ans(0);

    for(auto x: group){
        ans.push_back(0);
        if(x.size() == 1 && dif[x[0]] > 0){
            cout<<-1<<endl;
            return 0;
        }
        for(auto y : x){
            ans[ans.size() - 1] = max(ans[ans.size() - 1],dif[y] );
        }
    }

    int an = 0;
    for(auto a:ans){
        an += a;
    }

    cout<<an<<endl;
}