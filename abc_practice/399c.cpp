#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;

class cycle_base{
    private:
    struct edge {
        int to;
        int id;
    };

    struct undirected_edge {
        int from;
        int to;
    };

    struct dsu {
        vector<int> parent_or_size;

        dsu() {}
        explicit dsu(int n) : parent_or_size(n, -1) {}

        int leader(int a) {
            if(parent_or_size[a] < 0) return a;
            return parent_or_size[a] = leader(parent_or_size[a]);
        }

        int merge(int a, int b) {
            int x = leader(a), y = leader(b);
            if(x == y) return x;
            if(-parent_or_size[x] < -parent_or_size[y]) swap(x, y);
            parent_or_size[x] += parent_or_size[y];
            parent_or_size[y] = x;
            return x;
        }
    };

    struct flow_edge {
        int to;
        int rev;
        int cap;
        int original_edge_id;
        int from_vertex;
        int to_vertex;
    };

    int n;
    vector<undirected_edge> edges;
    vector<vector<edge>> g;

    bool built;
    int component_count;
    int cycle_component_count_value;

    vector<int> parent;
    vector<int> parent_edge;
    vector<int> depth;
    vector<int> component;
    vector<bool> is_tree_edge_value;

    vector<vector<int>> basis_edges_value;
    vector<vector<int>> basis_vertices_value;
    vector<int> chord_of_basis_value;
    vector<int> basis_of_chord_value;
    vector<vector<int>> basis_of_edge_value;
    vector<vector<int>> basis_of_vertex_value;
    vector<int> cycle_component_of_basis_value;
    vector<vector<int>> cycle_components_of_vertex_value;

    void check_vertex(int v) const {
        assert(0 <= v && v < n);
    }

    void check_edge(int e) const {
        assert(0 <= e && e < (int)edges.size());
    }

    void check_built() const {
        assert(built);
    }

    pair<vector<int>, vector<int>> restore_fundamental_cycle(int chord) const {
        int u = edges[chord].from;
        int v = edges[chord].to;
        int a = u, b = v;

        vector<int> left_edges, right_edges;
        vector<int> left_vertices, right_vertices;
        left_vertices.push_back(u);
        right_vertices.push_back(v);

        while(depth[a] > depth[b]) {
            left_edges.push_back(parent_edge[a]);
            a = parent[a];
            left_vertices.push_back(a);
        }
        while(depth[b] > depth[a]) {
            right_edges.push_back(parent_edge[b]);
            b = parent[b];
            right_vertices.push_back(b);
        }
        while(a != b) {
            left_edges.push_back(parent_edge[a]);
            a = parent[a];
            left_vertices.push_back(a);

            right_edges.push_back(parent_edge[b]);
            b = parent[b];
            right_vertices.push_back(b);
        }

        vector<int> cycle_edges = left_edges;
        for(int i = (int)right_edges.size() - 1; i >= 0; i--) {
            cycle_edges.push_back(right_edges[i]);
        }
        cycle_edges.push_back(chord);

        vector<int> cycle_vertices = left_vertices;
        for(int i = (int)right_vertices.size() - 2; i >= 0; i--) {
            cycle_vertices.push_back(right_vertices[i]);
        }

        return {cycle_edges, cycle_vertices};
    }

    void add_flow_edge(vector<vector<flow_edge>>& fg, int from, int to, int cap,
                       int original_edge_id = -1, int from_vertex = -1, int to_vertex = -1) const {
        flow_edge fwd = {to, (int)fg[to].size(), cap, original_edge_id, from_vertex, to_vertex};
        flow_edge rev = {from, (int)fg[from].size(), 0, -1, -1, -1};
        fg[from].push_back(fwd);
        fg[to].push_back(rev);
    }

    int max_flow_two_paths(vector<vector<flow_edge>>& fg, int source, int sink) const {
        int flow = 0;
        const int node_count = fg.size();
        vector<int> level(node_count), iter(node_count);

        auto bfs = [&]() -> bool {
            fill(level.begin(), level.end(), -1);
            queue<int> q;
            level[source] = 0;
            q.push(source);
            while(!q.empty()) {
                int v = q.front();
                q.pop();
                for(const auto& e : fg[v]) {
                    if(e.cap == 0 || level[e.to] != -1) continue;
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
            return level[sink] != -1;
        };

        auto dfs = [&](auto self, int v, int pushed) -> int {
            if(v == sink) return pushed;
            for(int& i = iter[v]; i < (int)fg[v].size(); i++) {
                flow_edge& e = fg[v][i];
                if(e.cap == 0 || level[e.to] != level[v] + 1) continue;
                int add = self(self, e.to, min(pushed, e.cap));
                if(add == 0) continue;
                e.cap -= add;
                fg[e.to][e.rev].cap += add;
                return add;
            }
            return 0LL;
        };

        while(flow < 2 && bfs()) {
            fill(iter.begin(), iter.end(), 0);
            while(flow < 2) {
                int pushed = dfs(dfs, source, 2 - flow);
                if(pushed == 0) break;
                flow += pushed;
            }
        }
        return flow;
    }

    vector<int> restore_flow_path(int s, int t, vector<vector<pair<int, int>>>& used) const {
        vector<int> path;

        auto dfs = [&](auto self, int v) -> bool {
            if(v == t) return true;
            while(!used[v].empty()) {
                auto [to, id] = used[v].back();
                used[v].pop_back();
                path.push_back(id);
                if(self(self, to)) return true;
                path.pop_back();
            }
            return false;
        };

        bool ok = dfs(dfs, s);
        if(!ok) return {};
        return path;
    }

    int first_common_cycle_component(int u, int v) const {
        const auto& a = cycle_components_of_vertex_value[u];
        const auto& b = cycle_components_of_vertex_value[v];
        int i = 0, j = 0;
        while(i < (int)a.size() && j < (int)b.size()) {
            if(a[i] == b[j]) return a[i];
            if(a[i] < b[j]) i++;
            else j++;
        }
        return -1;
    }

    public:
    explicit cycle_base(int n_)
        : n(n_), g(n_), built(false), component_count(0), cycle_component_count_value(0) {}

    void add_edge(int u, int v) {
        check_vertex(u);
        check_vertex(v);
        int id = edges.size();
        edges.push_back({u, v});
        g[u].push_back({v, id});
        g[v].push_back({u, id});
        built = false;
    }

    void build() {
        int m = edges.size();
        built = false;
        component_count = 0;
        cycle_component_count_value = 0;

        parent.assign(n, -1);
        parent_edge.assign(n, -1);
        depth.assign(n, 0);
        component.assign(n, -1);
        is_tree_edge_value.assign(m, false);
        basis_edges_value.clear();
        basis_vertices_value.clear();
        chord_of_basis_value.clear();
        basis_of_chord_value.assign(m, -1);
        basis_of_edge_value.assign(m, {});
        basis_of_vertex_value.assign(n, {});
        cycle_component_of_basis_value.clear();
        cycle_components_of_vertex_value.assign(n, {});

        for(int root = 0; root < n; root++) {
            if(component[root] != -1) continue;
            queue<int> q;
            component[root] = component_count;
            parent[root] = -1;
            parent_edge[root] = -1;
            depth[root] = 0;
            q.push(root);

            while(!q.empty()) {
                int v = q.front();
                q.pop();
                for(const auto& e : g[v]) {
                    if(component[e.to] != -1) continue;
                    component[e.to] = component_count;
                    parent[e.to] = v;
                    parent_edge[e.to] = e.id;
                    depth[e.to] = depth[v] + 1;
                    is_tree_edge_value[e.id] = true;
                    q.push(e.to);
                }
            }

            component_count++;
        }

        for(int id = 0; id < m; id++) {
            if(is_tree_edge_value[id]) continue;
            auto [cycle_edges, cycle_vertices] = restore_fundamental_cycle(id);
            int basis_id = basis_edges_value.size();
            basis_edges_value.push_back(cycle_edges);
            basis_vertices_value.push_back(cycle_vertices);
            chord_of_basis_value.push_back(id);
            basis_of_chord_value[id] = basis_id;
        }

        for(int i = 0; i < (int)basis_edges_value.size(); i++) {
            for(int e : basis_edges_value[i]) {
                basis_of_edge_value[e].push_back(i);
            }
            for(int v : basis_vertices_value[i]) {
                basis_of_vertex_value[v].push_back(i);
            }
        }

        dsu basis_dsu((int)basis_edges_value.size());
        for(int e = 0; e < m; e++) {
            const auto& bs = basis_of_edge_value[e];
            for(int i = 1; i < (int)bs.size(); i++) {
                basis_dsu.merge(bs[0], bs[i]);
            }
        }

        map<int, int> compressed;
        cycle_component_of_basis_value.assign(basis_edges_value.size(), -1);
        for(int i = 0; i < (int)basis_edges_value.size(); i++) {
            int leader = basis_dsu.leader(i);
            if(!compressed.count(leader)) {
                int id = compressed.size();
                compressed[leader] = id;
            }
            cycle_component_of_basis_value[i] = compressed[leader];
        }
        cycle_component_count_value = compressed.size();

        for(int v = 0; v < n; v++) {
            for(int b : basis_of_vertex_value[v]) {
                cycle_components_of_vertex_value[v].push_back(cycle_component_of_basis_value[b]);
            }
            sort(cycle_components_of_vertex_value[v].begin(), cycle_components_of_vertex_value[v].end());
            cycle_components_of_vertex_value[v].erase(
                unique(cycle_components_of_vertex_value[v].begin(), cycle_components_of_vertex_value[v].end()),
                cycle_components_of_vertex_value[v].end()
            );
        }

        built = true;
    }

    int rank() const {
        check_built();
        return basis_edges_value.size();
    }

    int connected_components() const {
        check_built();
        return component_count;
    }

    const vector<vector<int>>& basis_edges() const {
        check_built();
        return basis_edges_value;
    }

    const vector<vector<int>>& basis_vertices() const {
        check_built();
        return basis_vertices_value;
    }

    const vector<int>& bases_containing_edge(int e) const {
        check_built();
        check_edge(e);
        return basis_of_edge_value[e];
    }

    const vector<int>& bases_containing_vertex(int v) const {
        check_built();
        check_vertex(v);
        return basis_of_vertex_value[v];
    }

    bool in_some_cycle(int e) const {
        check_built();
        check_edge(e);
        return !basis_of_edge_value[e].empty();
    }

    bool same_cycle(int u, int v) const {
        check_built();
        check_vertex(u);
        check_vertex(v);
        return first_common_cycle_component(u, v) != -1;
    }

    vector<int> get_cycle(int u, int v) const {
        check_built();
        check_vertex(u);
        check_vertex(v);
        int common_component = first_common_cycle_component(u, v);
        if(common_component == -1) return {};

        if(u == v) {
            for(int b : basis_of_vertex_value[u]) {
                if(cycle_component_of_basis_value[b] == common_component) {
                    return basis_edges_value[b];
                }
            }
            return {};
        }

        vector<bool> edge_in_component(edges.size(), false);
        for(int b = 0; b < (int)basis_edges_value.size(); b++) {
            if(cycle_component_of_basis_value[b] != common_component) continue;
            for(int e : basis_edges_value[b]) edge_in_component[e] = true;
        }

        int flow_node_count = 2 * n;
        vector<vector<flow_edge>> fg(flow_node_count);
        auto vin = [&](int x) { return 2 * x; };
        auto vout = [&](int x) { return 2 * x + 1; };

        for(int x = 0; x < n; x++) {
            int cap = (x == u || x == v ? 2 : 1);
            add_flow_edge(fg, vin(x), vout(x), cap);
        }
        for(int id = 0; id < (int)edges.size(); id++) {
            if(!edge_in_component[id]) continue;
            int a = edges[id].from;
            int b = edges[id].to;
            if(a == b) continue;
            add_flow_edge(fg, vout(a), vin(b), 1, id, a, b);
            add_flow_edge(fg, vout(b), vin(a), 1, id, b, a);
        }

        int flow = max_flow_two_paths(fg, vout(u), vin(v));
        if(flow < 2) return {};

        vector<vector<pair<int, int>>> used(n);
        for(int from = 0; from < flow_node_count; from++) {
            for(const auto& e : fg[from]) {
                if(e.original_edge_id == -1) continue;
                if(e.cap != 0) continue;
                used[e.from_vertex].push_back({e.to_vertex, e.original_edge_id});
            }
        }

        vector<int> path1 = restore_flow_path(u, v, used);
        vector<int> path2 = restore_flow_path(u, v, used);
        if(path1.empty() || path2.empty()) return {};

        vector<int> cycle = path1;
        for(int i = (int)path2.size() - 1; i >= 0; i--) {
            cycle.push_back(path2[i]);
        }
        return cycle;
    }

    vector<bool> coordinate(const vector<int>& edge_set) const {
        check_built();
        vector<bool> used(edges.size(), false);
        for(int e : edge_set) {
            check_edge(e);
            used[e] = true;
        }

        vector<bool> coef(basis_edges_value.size(), false);
        for(int i = 0; i < (int)basis_edges_value.size(); i++) {
            coef[i] = used[chord_of_basis_value[i]];
        }
        return coef;
    }

    vector<int> compose(const vector<bool>& coef) const {
        check_built();
        assert((int)coef.size() == (int)basis_edges_value.size());
        vector<bool> used(edges.size(), false);
        for(int i = 0; i < (int)basis_edges_value.size(); i++) {
            if(!coef[i]) continue;
            for(int e : basis_edges_value[i]) {
                used[e] = !used[e];
            }
        }

        vector<int> result;
        for(int e = 0; e < (int)edges.size(); e++) {
            if(used[e]) result.push_back(e);
        }
        return result;
    }

    int cycle_component_count() const {
        check_built();
        return cycle_component_count_value;
    }

    int cycle_component_of_basis(int i) const {
        check_built();
        assert(0 <= i && i < (int)cycle_component_of_basis_value.size());
        return cycle_component_of_basis_value[i];
    }

    const vector<int>& cycle_components_of_vertex(int v) const {
        check_built();
        check_vertex(v);
        return cycle_components_of_vertex_value[v];
    }
};

lint main(){
    int n, m;
    cin>>n>>m;
    cycle_base cy(n);
    rep(i,m){
        int a, b;
        cin>>a>>b;
        a--, b--;
        cy.add_edge(a,b);
    }
    cy.build();
    cout<<cy.rank()<<endl;
}