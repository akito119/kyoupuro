#include <bits/stdc++.h>
using namespace std;

// Implicit fundamental-cycle basis for an undirected multigraph.
class cycle_base {
    struct edge_data { int u, v; long long weight; };
    struct dsu {
        vector<int> p;
        explicit dsu(int n = 0) : p(n, -1) {}
        int leader(int v) { return p[v] < 0 ? v : p[v] = leader(p[v]); }
        int leader_const(int v) const { while(p[v] >= 0) v = p[v]; return v; }
        int size(int v) { return -p[leader(v)]; }
        bool same(int a, int b) { return leader(a) == leader(b); }
        int merge(int a, int b) {
            a = leader(a); b = leader(b);
            if(a == b) return a;
            if(-p[a] < -p[b]) swap(a, b);
            p[a] += p[b]; p[b] = a;
            return a;
        }
    } uf;

    int n, log_n, components_value, odd_cycle_basis_value = -1;
    vector<edge_data> edges;
    vector<vector<pair<int, int>>> forest;
    vector<char> tree_edge;
    vector<int> chord_of_basis_value, basis_of_chord_value;
    vector<int> parent, parent_edge, depth;
    vector<vector<int>> up;
    vector<long long> dist;
    vector<int> component_edges;

    void check_vertex(int v) const { assert(0 <= v && v < n); }
    void check_edge(int e) const { assert(0 <= e && e < (int)edges.size()); }
    void check_basis(int b) const { assert(0 <= b && b < rank()); }
    void set_ancestors(int v, int p) {
        up[v][0] = p;
        for(int k = 1; k < log_n; k++)
            up[v][k] = up[v][k - 1] == -1 ? -1 : up[up[v][k - 1]][k - 1];
    }
    void link_forest(int a, int b, int edge_id) {
        if(uf.size(a) > uf.size(b)) swap(a, b);
        struct state { int v, p, pe, d; long long w; };
        vector<state> stack = {{a, b, edge_id, depth[b] + 1,
                                dist[b] + edges[edge_id].weight}};
        while(!stack.empty()) {
            auto [v, p, pe, d, w] = stack.back(); stack.pop_back();
            parent[v] = p; parent_edge[v] = pe; depth[v] = d; dist[v] = w;
            set_ancestors(v, p);
            for(auto [to, id] : forest[v]) if(to != p)
                stack.push_back({to, v, id, d + 1, w + edges[id].weight});
        }
        int ra = uf.leader(a), rb = uf.leader(b);
        int total = component_edges[ra] + component_edges[rb] + 1;
        int root = uf.merge(ra, rb);
        component_edges[root] = total;
        forest[a].push_back({b, edge_id});
        forest[b].push_back({a, edge_id});
        components_value--;
    }
    int lca(int a, int b) const {
        assert(uf.leader_const(a) == uf.leader_const(b));
        if(depth[a] < depth[b]) swap(a, b);
        int diff = depth[a] - depth[b];
        for(int k = 0; k < log_n; k++) if(diff >> k & 1) a = up[a][k];
        if(a == b) return a;
        for(int k = log_n - 1; k >= 0; k--) if(up[a][k] != up[b][k]) {
            a = up[a][k]; b = up[b][k];
        }
        return parent[a];
    }

public:
    struct basis_descriptor { int chord, u, v; };
    explicit cycle_base(int n_)
        : uf(n_), n(n_), log_n(1), components_value(n_), forest(n_),
          parent(n_, -1), parent_edge(n_, -1), depth(n_, 0), dist(n_, 0),
          component_edges(n_, 0) {
        assert(n >= 0);
        while((1LL << log_n) <= max(1, n)) log_n++;
        up.assign(n, vector<int>(log_n, -1));
    }
    void reserve_edges(int m) {
        edges.reserve(m); tree_edge.reserve(m);
        basis_of_chord_value.reserve(m); chord_of_basis_value.reserve(m);
    }

    // All additions: O(Q alpha(N) + N log^2 N).  A returned witness additionally
    // costs its output size.
    int add_edge(int u, int v, long long weight = 1) {
        check_vertex(u); check_vertex(v);
        int id = edge_count();
        edges.push_back({u, v, weight});
        tree_edge.push_back(false);
        basis_of_chord_value.push_back(-1);
        if(!uf.same(u, v)) {
            tree_edge[id] = true;
            link_forest(u, v, id);
        } else {
            component_edges[uf.leader(u)]++;
            basis_of_chord_value[id] = rank();
            chord_of_basis_value.push_back(id);
            if(odd_cycle_basis_value == -1 && ((depth[u] ^ depth[v]) & 1) == 0)
                odd_cycle_basis_value = basis_of_chord_value[id];
        }
        return id;
    }
    void build() {} // compatibility: maintained online

    int vertex_count() const { return n; }
    int edge_count() const { return edges.size(); }
    int rank() const { return chord_of_basis_value.size(); }
    int connected_components() const { return components_value; }
    bool connected(int u, int v) { check_vertex(u); check_vertex(v); return uf.same(u, v); }
    bool would_create_cycle(int u, int v) { return connected(u, v); }
    bool is_tree_edge(int e) const { check_edge(e); return tree_edge[e]; }
    pair<int, int> edge_endpoints(int e) const { check_edge(e); return {edges[e].u, edges[e].v}; }
    long long edge_weight(int e) const { check_edge(e); return edges[e].weight; }
    int chord_of_basis(int b) const { check_basis(b); return chord_of_basis_value[b]; }
    int basis_of_chord(int e) const { check_edge(e); return basis_of_chord_value[e]; }
    basis_descriptor basis(int b) const {
        check_basis(b); int e = chord_of_basis_value[b];
        return {e, edges[e].u, edges[e].v};
    }
    int component_rank(int v) {
        check_vertex(v); int r = uf.leader(v);
        return component_edges[r] - uf.size(r) + 1;
    }
    bool component_is_tree(int v) { return component_rank(v) == 0; }
    bool component_is_unicyclic(int v) { return component_rank(v) == 1; }
    bool is_bipartite() const { return odd_cycle_basis_value == -1; }
    int odd_cycle_basis() const { return odd_cycle_basis_value; }
    vector<int> odd_cycle_edges() const {
        return is_bipartite() ? vector<int>{} : materialize_basis_edges(odd_cycle_basis_value);
    }

    int basis_cycle_size(int b) const {
        auto [e, u, v] = basis(b); int w = lca(u, v);
        return depth[u] + depth[v] - 2 * depth[w] + 1;
    }
    long long basis_cycle_weight(int b) const {
        auto [e, u, v] = basis(b); int w = lca(u, v);
        return dist[u] + dist[v] - 2 * dist[w] + edges[e].weight;
    }
    vector<int> materialize_basis_edges(int b) const {
        auto [chord, u, v] = basis(b); int w = lca(u, v);
        vector<int> left, right;
        while(u != w) { left.push_back(parent_edge[u]); u = parent[u]; }
        while(v != w) { right.push_back(parent_edge[v]); v = parent[v]; }
        reverse(right.begin(), right.end());
        left.insert(left.end(), right.begin(), right.end());
        left.push_back(chord);
        return left;
    }
    vector<int> materialize_basis_vertices(int b) const {
        auto [chord, u, v] = basis(b); int w = lca(u, v);
        vector<int> left, right;
        while(u != w) { left.push_back(u); u = parent[u]; }
        left.push_back(w);
        while(v != w) { right.push_back(v); v = parent[v]; }
        reverse(right.begin(), right.end());
        left.insert(left.end(), right.begin(), right.end());
        return left;
    }

    vector<int> odd_degree_vertices(const vector<int>& edge_set) const {
        vector<char> odd(n, false);
        for(int e : edge_set) {
            check_edge(e);
            odd[edges[e].u] ^= 1;
            odd[edges[e].v] ^= 1;
        }
        vector<int> result;
        for(int v = 0; v < n; v++) if(odd[v]) result.push_back(v);
        return result;
    }
    bool is_cycle_space_element(const vector<int>& s) const { return odd_degree_vertices(s).empty(); }
    vector<bool> coordinate_unchecked(const vector<int>& edge_set) const {
        vector<bool> result(rank(), false);
        for(int e : edge_set) {
            check_edge(e); int b = basis_of_chord_value[e];
            if(b != -1) result[b] = !result[b];
        }
        return result;
    }
    vector<bool> coordinate(const vector<int>& edge_set) const {
        assert(is_cycle_space_element(edge_set));
        return coordinate_unchecked(edge_set);
    }
    vector<int> sparse_coordinate(const vector<int>& edge_set) const {
        assert(is_cycle_space_element(edge_set));
        vector<int> ids;
        for(int e : edge_set) {
            check_edge(e);
            if(basis_of_chord_value[e] != -1) ids.push_back(basis_of_chord_value[e]);
        }
        sort(ids.begin(), ids.end());
        vector<int> result;
        for(int i = 0; i < (int)ids.size();) {
            int j = i + 1;
            while(j < (int)ids.size() && ids[j] == ids[i]) j++;
            if((j - i) & 1) result.push_back(ids[i]);
            i = j;
        }
        return result;
    }

    // Tree difference: O(N + rank + answer), independent of total basis length.
    vector<int> compose(const vector<bool>& coefficient) const {
        assert((int)coefficient.size() == rank());
        vector<char> delta(n, false); vector<int> result;
        for(int b = 0; b < rank(); b++) if(coefficient[b]) {
            int e = chord_of_basis_value[b]; result.push_back(e);
            delta[edges[e].u] ^= 1; delta[edges[e].v] ^= 1;
        }
        vector<vector<int>> children(n); vector<int> order;
        order.reserve(n);
        for(int v = 0; v < n; v++) {
            if(parent[v] == -1) order.push_back(v);
            else children[parent[v]].push_back(v);
        }
        for(int i = 0; i < (int)order.size(); i++)
            for(int to : children[order[i]]) order.push_back(to);
        for(int i = n - 1; i >= 0; i--) {
            int v = order[i];
            if(parent[v] != -1 && delta[v]) {
                result.push_back(parent_edge[v]); delta[parent[v]] ^= 1;
            }
        }
        return result;
    }
    vector<int> compose_sparse(const vector<int>& basis_ids) const {
        vector<int> ids = basis_ids;
        for(int b : ids) check_basis(b);
        sort(ids.begin(), ids.end());
        vector<char> delta(n, false);
        vector<int> result;
        for(int i = 0; i < (int)ids.size();) {
            int j = i + 1;
            while(j < (int)ids.size() && ids[j] == ids[i]) j++;
            if((j - i) & 1) {
                int e = chord_of_basis_value[ids[i]];
                result.push_back(e);
                delta[edges[e].u] ^= 1;
                delta[edges[e].v] ^= 1;
            }
            i = j;
        }
        vector<vector<int>> children(n);
        vector<int> order;
        order.reserve(n);
        for(int v = 0; v < n; v++) {
            if(parent[v] == -1) order.push_back(v);
            else children[parent[v]].push_back(v);
        }
        for(int i = 0; i < (int)order.size(); i++)
            for(int to : children[order[i]]) order.push_back(to);
        for(int i = n - 1; i >= 0; i--) {
            int v = order[i];
            if(parent[v] != -1 && delta[v]) {
                result.push_back(parent_edge[v]);
                delta[parent[v]] ^= 1;
            }
        }
        return result;
    }

    // Solves boundary(x)=odd_vertices inside the affine space whose chord
    // coordinates are coefficient.  nullopt means that some connected
    // component contains an odd number of requested boundary vertices.
    optional<vector<int>> compose_with_boundary(
        const vector<int>& odd_vertices, const vector<bool>& coefficient) const {
        assert((int)coefficient.size() == rank());
        vector<char> delta(n, false);
        vector<int> result;
        for(int v : odd_vertices) { check_vertex(v); delta[v] ^= 1; }
        for(int b = 0; b < rank(); b++) if(coefficient[b]) {
            int e = chord_of_basis_value[b];
            result.push_back(e);
            delta[edges[e].u] ^= 1;
            delta[edges[e].v] ^= 1;
        }
        vector<vector<int>> children(n);
        vector<int> order;
        order.reserve(n);
        for(int v = 0; v < n; v++) {
            if(parent[v] == -1) order.push_back(v);
            else children[parent[v]].push_back(v);
        }
        for(int i = 0; i < (int)order.size(); i++)
            for(int to : children[order[i]]) order.push_back(to);
        for(int i = n - 1; i >= 0; i--) {
            int v = order[i];
            if(parent[v] == -1) {
                if(delta[v]) return nullopt;
            } else if(delta[v]) {
                result.push_back(parent_edge[v]);
                delta[parent[v]] ^= 1;
            }
        }
        return result;
    }
    optional<vector<int>> chain_from_coordinate(
        int s, int t, const vector<bool>& coefficient) const {
        check_vertex(s); check_vertex(t);
        return compose_with_boundary({s, t}, coefficient);
    }
    bool boundary_feasible(const vector<int>& odd_vertices) const {
        vector<char> parity(n, false);
        for(int v : odd_vertices) {
            check_vertex(v);
            parity[uf.leader_const(v)] ^= 1;
        }
        for(char x : parity) if(x) return false;
        return true;
    }
    long long boundary_fiber_size_mod(
        const vector<int>& odd_vertices, long long mod) const {
        assert(mod > 0);
        return boundary_feasible(odd_vertices) ? cycle_space_size_mod(mod) : 0;
    }
    long long cycle_space_size_mod(long long mod) const {
        assert(mod > 0); long long a = 2 % mod, e = rank(), result = 1 % mod;
        while(e) {
            if(e & 1) result = (__int128)result * a % mod;
            a = (__int128)a * a % mod; e >>= 1;
        }
        return result;
    }
};

// Terminal-preserving leaf pruning and degree-2 contraction.  It preserves all
// terminal paths but may retain cyclic blocks attached at one articulation.
// Each kernel edge remembers its original edge path.
class cycle_core {
public:
    struct original_edge { int u, v; long long weight; };
    struct core_edge {
        int u, v;
        long long weight;
        vector<int> original_edges;
    };
    struct result {
        vector<int> original_vertex; // core vertex -> original vertex
        vector<int> core_vertex;     // original vertex -> core vertex, or -1
        vector<core_edge> edges;
        vector<char> active_original_vertex;
    };

private:
    int n;
    vector<original_edge> edges;
    vector<vector<pair<int, int>>> graph;

public:
    explicit cycle_core(int n_) : n(n_), graph(n_) { assert(n >= 0); }
    int add_edge(int u, int v, long long weight = 1) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        int id = edges.size();
        edges.push_back({u, v, weight});
        graph[u].push_back({v, id});
        graph[v].push_back({u, id});
        return id;
    }

    // Nonterminal leaves are irrelevant to terminal-to-terminal simple paths.
    // Remaining non-junction degree-2 paths are contracted into weighted edges.
    result compress(const vector<int>& terminals) const {
        vector<char> terminal(n, false), active(n, true);
        for(int v : terminals) { assert(0 <= v && v < n); terminal[v] = true; }
        vector<int> degree(n);
        queue<int> q;
        for(int v = 0; v < n; v++) {
            degree[v] = graph[v].size();
            if(!terminal[v] && degree[v] <= 1) q.push(v);
        }
        while(!q.empty()) {
            int v = q.front(); q.pop();
            if(!active[v] || terminal[v] || degree[v] > 1) continue;
            active[v] = false;
            for(auto [to, id] : graph[v]) if(active[to]) {
                if(--degree[to] <= 1 && !terminal[to]) q.push(to);
            }
        }

        vector<char> junction(n, false), seen(n, false);
        for(int v = 0; v < n; v++)
            if(active[v] && (terminal[v] || degree[v] != 2)) junction[v] = true;

        // A terminal-free pure-cycle component has no natural junction.  Pick
        // one representative so the cycle becomes a kernel self-loop.
        for(int s = 0; s < n; s++) if(active[s] && !seen[s]) {
            vector<int> component;
            vector<int> stack = {s}; seen[s] = true;
            bool has_junction = false;
            while(!stack.empty()) {
                int v = stack.back(); stack.pop_back();
                component.push_back(v); has_junction |= junction[v];
                for(auto [to, id] : graph[v]) if(active[to] && !seen[to]) {
                    seen[to] = true; stack.push_back(to);
                }
            }
            if(!has_junction) junction[component[0]] = true;
        }

        result out;
        out.core_vertex.assign(n, -1);
        out.active_original_vertex = active;
        for(int v = 0; v < n; v++) if(junction[v]) {
            out.core_vertex[v] = out.original_vertex.size();
            out.original_vertex.push_back(v);
        }

        vector<char> used(edges.size(), false);
        for(int s : out.original_vertex) {
            for(auto [first_to, first_edge] : graph[s]) {
                if(!active[first_to] || used[first_edge]) continue;
                vector<int> path;
                long long weight = 0;
                int v = s, to = first_to, id = first_edge;
                while(true) {
                    used[id] = true;
                    path.push_back(id);
                    weight += edges[id].weight;
                    v = to;
                    if(junction[v]) break;
                    int next_to = -1, next_id = -1;
                    for(auto [candidate, eid] : graph[v])
                        if(active[candidate] && eid != id) {
                            next_to = candidate; next_id = eid; break;
                        }
                    assert(next_id != -1);
                    to = next_to; id = next_id;
                }
                out.edges.push_back({out.core_vertex[s], out.core_vertex[v],
                                     weight, move(path)});
            }
        }
        return out;
    }
};

// A bit-parallel cycle space for a graph with at most 64 edges.  Intended for
// exhaustive algorithms parameterized by the cyclomatic rank after cycle_core.
class small_cycle_space64 {
    struct edge_data { int u, v; long long weight; };
    struct dsu {
        vector<int> p;
        explicit dsu(int n) : p(n, -1) {}
        int leader(int v) { return p[v] < 0 ? v : p[v] = leader(p[v]); }
        bool merge(int a, int b) {
            a = leader(a); b = leader(b);
            if(a == b) return false;
            if(-p[a] < -p[b]) swap(a, b);
            p[a] += p[b]; p[b] = a;
            return true;
        }
    };

    int n;
    bool built = false;
    vector<edge_data> edges;
    vector<vector<pair<int, int>>> graph, forest;
    vector<int> parent, parent_edge, depth, order;
    vector<char> tree_edge;
    vector<int> chord_of_basis;
    vector<uint64_t> basis_masks;

    void check_built() const { assert(built); }
    void check_vertex(int v) const { assert(0 <= v && v < n); }

public:
    explicit small_cycle_space64(int n_) : n(n_), graph(n_), forest(n_) {
        assert(n >= 0);
    }
    explicit small_cycle_space64(const cycle_core::result& core)
        : small_cycle_space64((int)core.original_vertex.size()) {
        for(const auto& e : core.edges) add_edge(e.u, e.v, e.weight);
        build();
    }
    int add_edge(int u, int v, long long weight = 1) {
        check_vertex(u); check_vertex(v);
        assert(edges.size() < 64);
        int id = edges.size();
        edges.push_back({u, v, weight});
        graph[u].push_back({v, id});
        graph[v].push_back({u, id});
        built = false;
        return id;
    }
    void build() {
        int m = edges.size();
        dsu uf(n);
        forest.assign(n, {});
        tree_edge.assign(m, false);
        chord_of_basis.clear();
        for(int e = 0; e < m; e++) {
            auto [u, v, weight] = edges[e];
            if(uf.merge(u, v)) {
                tree_edge[e] = true;
                forest[u].push_back({v, e});
                forest[v].push_back({u, e});
            } else chord_of_basis.push_back(e);
        }

        parent.assign(n, -1);
        parent_edge.assign(n, -1);
        depth.assign(n, 0);
        order.clear();
        vector<char> seen(n, false);
        for(int root = 0; root < n; root++) if(!seen[root]) {
            seen[root] = true;
            vector<int> stack = {root};
            while(!stack.empty()) {
                int v = stack.back(); stack.pop_back();
                order.push_back(v);
                for(auto [to, e] : forest[v]) if(to != parent[v]) {
                    parent[to] = v;
                    parent_edge[to] = e;
                    depth[to] = depth[v] + 1;
                    seen[to] = true;
                    stack.push_back(to);
                }
            }
        }

        basis_masks.clear();
        for(int chord : chord_of_basis) {
            int u = edges[chord].u, v = edges[chord].v;
            uint64_t mask = uint64_t(1) << chord;
            while(depth[u] > depth[v]) {
                mask ^= uint64_t(1) << parent_edge[u]; u = parent[u];
            }
            while(depth[v] > depth[u]) {
                mask ^= uint64_t(1) << parent_edge[v]; v = parent[v];
            }
            while(u != v) {
                mask ^= uint64_t(1) << parent_edge[u]; u = parent[u];
                mask ^= uint64_t(1) << parent_edge[v]; v = parent[v];
            }
            basis_masks.push_back(mask);
        }
        built = true;
    }

    int edge_count() const { return edges.size(); }
    int rank() const { check_built(); return basis_masks.size(); }
    uint64_t basis_mask(int b) const {
        check_built(); assert(0 <= b && b < rank()); return basis_masks[b];
    }
    uint64_t compose_mask(uint64_t coordinate) const {
        check_built();
        if(rank() < 64) assert((coordinate >> rank()) == 0);
        uint64_t result = 0;
        while(coordinate) {
            int b = __builtin_ctzll(coordinate);
            result ^= basis_masks[b];
            coordinate &= coordinate - 1;
        }
        return result;
    }
    uint64_t coordinate_mask(uint64_t edge_mask) const {
        check_built();
        if(edge_count() < 64) assert((edge_mask >> edge_count()) == 0);
        uint64_t coordinate = 0;
        for(int b = 0; b < rank(); b++)
            if(edge_mask >> chord_of_basis[b] & 1) coordinate |= uint64_t(1) << b;
        return coordinate;
    }
    bool is_cycle_mask(uint64_t edge_mask) const {
        check_built();
        if(edge_count() < 64) assert((edge_mask >> edge_count()) == 0);
        vector<char> odd(n, false);
        for(int e = 0; e < edge_count(); e++) if(edge_mask >> e & 1) {
            odd[edges[e].u] ^= 1;
            odd[edges[e].v] ^= 1;
        }
        return find(odd.begin(), odd.end(), true) == odd.end();
    }

    optional<uint64_t> affine_mask(
        const vector<int>& odd_vertices, uint64_t coordinate = 0) const {
        check_built();
        uint64_t result = compose_mask(coordinate);
        vector<char> delta(n, false);
        for(int v : odd_vertices) { check_vertex(v); delta[v] ^= 1; }
        for(int i = (int)order.size() - 1; i >= 0; i--) {
            int v = order[i];
            if(parent[v] == -1) {
                if(delta[v]) return nullopt;
            } else if(delta[v]) {
                result ^= uint64_t(1) << parent_edge[v];
                delta[parent[v]] ^= 1;
            }
        }
        return result;
    }

    // Enumerates an affine fiber in Gray-code order.  Successive masks differ
    // by exactly one fundamental cycle XOR.  max_rank prevents accidental blowup.
    template<class Callback>
    bool enumerate_affine(const vector<int>& odd_vertices, int max_rank,
                          Callback callback) const {
        check_built();
        assert(0 <= max_rank && max_rank <= 62);
        if(rank() > max_rank) return false;
        auto initial = affine_mask(odd_vertices, 0);
        if(!initial) return false;
        uint64_t current = *initial;
        uint64_t count = uint64_t(1) << rank();
        callback(current, uint64_t(0));
        for(uint64_t i = 1; i < count; i++) {
            int changed_basis = __builtin_ctzll(i);
            current ^= basis_masks[changed_basis];
            uint64_t gray_coordinate = i ^ (i >> 1);
            callback(current, gray_coordinate);
        }
        return true;
    }

    bool is_simple_path(uint64_t mask, int s, int t) const {
        check_built(); check_vertex(s); check_vertex(t);
        if(edge_count() < 64) assert((mask >> edge_count()) == 0);
        if(s == t) return false;
        vector<int> degree(n, 0);
        for(int e = 0; e < edge_count(); e++) if(mask >> e & 1) {
            int u = edges[e].u, v = edges[e].v;
            if(u == v) degree[u] += 2;
            else { degree[u]++; degree[v]++; }
        }
        if(degree[s] != 1 || degree[t] != 1) return false;
        for(int v = 0; v < n; v++)
            if(v != s && v != t && degree[v] != 0 && degree[v] != 2) return false;

        vector<char> seen(n, false);
        vector<int> stack = {s}; seen[s] = true;
        while(!stack.empty()) {
            int v = stack.back(); stack.pop_back();
            for(auto [to, e] : graph[v])
                if((mask >> e & 1) && !seen[to]) { seen[to] = true; stack.push_back(to); }
        }
        if(!seen[t]) return false;
        for(int e = 0; e < edge_count(); e++) if(mask >> e & 1)
            if(!seen[edges[e].u]) return false;
        return true;
    }
    long long mask_weight(uint64_t mask) const {
        check_built();
        if(edge_count() < 64) assert((mask >> edge_count()) == 0);
        long long result = 0;
        for(int e = 0; e < edge_count(); e++) if(mask >> e & 1) result += edges[e].weight;
        return result;
    }
};

// Incremental bridges and 2-edge-connected components.
class incremental_bridge_graph {
    int n, bridges_value = 0, lca_iteration = 0;
    vector<int> dsu_2ecc, dsu_cc, cc_size, parent, parent_edge, last_visit;
    vector<char> bridge;
    vector<int> cover_edge, cyclic_at;
    int find_2ecc(int v) {
        if(v == -1) return -1;
        return dsu_2ecc[v] == v ? v : dsu_2ecc[v] = find_2ecc(dsu_2ecc[v]);
    }
    int find_cc(int v) {
        v = find_2ecc(v);
        return dsu_cc[v] == v ? v : dsu_cc[v] = find_cc(dsu_cc[v]);
    }
    void make_root(int v) {
        v = find_2ecc(v);
        int root = v, child = -1, child_edge = -1;
        while(v != -1) {
            int p = find_2ecc(parent[v]), pe = parent_edge[v];
            parent[v] = child; parent_edge[v] = child_edge; dsu_cc[v] = root;
            child = v; child_edge = pe; v = p;
        }
        cc_size[root] = cc_size[child];
    }
    void remove_bridge(int e, int witness) {
        if(e == -1 || !bridge[e]) return;
        bridge[e] = false; cover_edge[e] = witness; cyclic_at[e] = witness; bridges_value--;
    }
    void merge_path(int a, int b, int witness) {
        ++lca_iteration; vector<int> pa, pb; int lca = -1;
        while(lca == -1) {
            if(a != -1) {
                a = find_2ecc(a); pa.push_back(a);
                if(last_visit[a] == lca_iteration) { lca = a; break; }
                last_visit[a] = lca_iteration; a = parent[a];
            }
            if(b != -1) {
                b = find_2ecc(b); pb.push_back(b);
                if(last_visit[b] == lca_iteration) { lca = b; break; }
                last_visit[b] = lca_iteration; b = parent[b];
            }
        }
        for(int v : pa) { dsu_2ecc[v] = lca; if(v == lca) break; remove_bridge(parent_edge[v], witness); }
        for(int v : pb) { dsu_2ecc[v] = lca; if(v == lca) break; remove_bridge(parent_edge[v], witness); }
    }
public:
    explicit incremental_bridge_graph(int n_)
        : n(n_), dsu_2ecc(n_), dsu_cc(n_), cc_size(n_, 1), parent(n_, -1),
          parent_edge(n_, -1), last_visit(n_, 0) {
        iota(dsu_2ecc.begin(), dsu_2ecc.end(), 0);
        iota(dsu_cc.begin(), dsu_cc.end(), 0);
    }
    int add_edge(int a, int b) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        int id = edge_count(); bridge.push_back(false);
        cover_edge.push_back(id); cyclic_at.push_back(id);
        int x = find_2ecc(a), y = find_2ecc(b);
        if(x == y) return id;
        int ca = find_cc(x), cb = find_cc(y);
        if(ca != cb) {
            bridge[id] = true; cover_edge[id] = cyclic_at[id] = -1; bridges_value++;
            if(cc_size[ca] > cc_size[cb]) { swap(x, y); swap(ca, cb); }
            make_root(x); parent[x] = y; parent_edge[x] = id;
            dsu_cc[x] = y; cc_size[cb] += cc_size[x];
        } else merge_path(x, y, id);
        return id;
    }
    int edge_count() const { return bridge.size(); }
    int bridge_count() const { return bridges_value; }
    bool is_bridge(int e) const { assert(0 <= e && e < edge_count()); return bridge[e]; }
    bool connected(int a, int b) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        return find_cc(a) == find_cc(b);
    }
    bool same_2edge_component(int a, int b) {
        assert(0 <= a && a < n && 0 <= b && b < n);
        return find_2ecc(a) == find_2ecc(b);
    }
    int cycle_witness_edge(int e) const {
        assert(0 <= e && e < edge_count()); return bridge[e] ? -1 : cover_edge[e];
    }
    int became_cyclic_at(int e) const {
        assert(0 <= e && e < edge_count()); return cyclic_at[e];
    }
};

// The image of a graph cycle space under XOR edge labels.
template<class UInt = uint64_t, int Bits = numeric_limits<UInt>::digits>
class xor_cycle_basis {
    static_assert(is_unsigned_v<UInt>);
    static_assert(0 < Bits && Bits <= numeric_limits<UInt>::digits);
    static constexpr UInt value_mask() {
        if constexpr(Bits == numeric_limits<UInt>::digits) return ~UInt(0);
        else return (UInt(1) << Bits) - 1;
    }
    static bool valid_value(UInt x) { return (x & ~value_mask()) == 0; }
    struct linear_basis {
        array<UInt, Bits> a{}; int dimension = 0;
        bool insert(UInt x) {
            for(int k = Bits - 1; k >= 0; k--) if(x >> k & UInt(1)) {
                if(!a[k]) { a[k] = x; dimension++; return true; }
                x ^= a[k];
            }
            return false;
        }
        bool contains(UInt x) const {
            for(int k = Bits - 1; k >= 0; k--) if(x >> k & UInt(1)) {
                if(!a[k]) return false; x ^= a[k];
            }
            return true;
        }
        UInt maximize(UInt x) const { for(int k = Bits - 1; k >= 0; k--) x = max(x, x ^ a[k]); return x; }
        UInt minimize(UInt x) const { for(int k = Bits - 1; k >= 0; k--) x = min(x, x ^ a[k]); return x; }
        void merge_from(const linear_basis& o) { for(UInt x : o.a) if(x) insert(x); }
    };
    int n;
    vector<int> parent, size_value;
    vector<int> anchor;
    vector<UInt> xor_to_parent;
    vector<linear_basis> cycles;
    pair<int, UInt> find(int v) {
        if(parent[v] == v) return {v, 0};
        auto [r, x] = find(parent[v]);
        xor_to_parent[v] ^= x; parent[v] = r;
        return {r, xor_to_parent[v]};
    }
public:
    explicit xor_cycle_basis(int n_)
        : n(n_), parent(n_), size_value(n_, 1), anchor(n_),
          xor_to_parent(n_, 0), cycles(n_) {
        iota(parent.begin(), parent.end(), 0);
        iota(anchor.begin(), anchor.end(), 0);
    }
    optional<UInt> add_edge(int u, int v, UInt label) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        assert(valid_value(label));
        auto [ru, xu] = find(u); auto [rv, xv] = find(v);
        UInt value = xu ^ xv ^ label;
        if(ru == rv) { cycles[ru].insert(value); return value; }
        if(size_value[ru] < size_value[rv]) swap(ru, rv);
        parent[rv] = ru; xor_to_parent[rv] = value;
        size_value[ru] += size_value[rv];
        anchor[ru] = min(anchor[ru], anchor[rv]);
        cycles[ru].merge_from(cycles[rv]);
        return nullopt;
    }
    bool connected(int u, int v) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        return find(u).first == find(v).first;
    }
    int image_rank(int v) {
        assert(0 <= v && v < n);
        return cycles[find(v).first].dimension;
    }
    bool cycle_space_xor_representable(int v, UInt x) {
        assert(0 <= v && v < n && valid_value(x));
        return cycles[find(v).first].contains(x);
    }
    UInt normalize_cycle_coset(int v, UInt x) {
        assert(0 <= v && v < n && valid_value(x));
        return cycles[find(v).first].minimize(x);
    }
    UInt canonical_vertex_value(int v) {
        assert(0 <= v && v < n);
        auto [root, potential] = find(v);
        auto [same_root, anchor_potential] = find(anchor[root]);
        assert(root == same_root);
        return cycles[root].minimize(potential ^ anchor_potential);
    }
    UInt maximum_walk_xor(int u, int v) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        auto [ru, xu] = find(u); auto [rv, xv] = find(v); assert(ru == rv);
        return cycles[ru].maximize(xu ^ xv);
    }
    UInt minimum_walk_xor(int u, int v) {
        assert(0 <= u && u < n && 0 <= v && v < n);
        auto [ru, xu] = find(u); auto [rv, xv] = find(v); assert(ru == rv);
        return cycles[ru].minimize(xu ^ xv);
    }
};
