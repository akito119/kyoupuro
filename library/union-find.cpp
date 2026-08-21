#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;


struct dsu {
    private:
    int _n;
    vector<int> parent_or_size;

    int _leader(int a) {
        if (parent_or_size[a] < 0) return a;
        return parent_or_size[a] = _leader(parent_or_size[a]);
    }

    public:
    dsu() : _n(0) {}
    explicit dsu(int n) : _n(n),parent_or_size(n, -1) {}

     int leader(int a) {
        assert(0 <= a && a < _n);
        return _leader(a);
    }

    int merge(int a, int b) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        int x = leader(a), y = leader(b);
        if(x == y) return x;
        if(-parent_or_size[x] < -parent_or_size[y]) swap(x, y);
        parent_or_size[x] += parent_or_size[y];
        parent_or_size[y] = x;
        return x;
    }

    bool same(int a, int b) {
        assert(0 <= a && a < _n);
        assert(0 <= b && b < _n);
        return leader(a) == leader(b);
    }

    int size(int a) {
        assert(0 <= a && a < _n);
        return -parent_or_size[leader(a)];
    }

    vector<vector<int>> groups() {
        vector<int> leader_buf(_n), group_size(_n);
        rep(i, _n){
            leader_buf[i] = leader(i);
            group_size[leader_buf[i]]++;
        }
        vector<vector<int>> result(_n);
        rep(i, _n) result[i].reserve(group_size[i]);
        rep(i, _n) result[leader_buf[i]].push_back(i);

        result.erase(
        remove_if(result.begin(), result.end(),
                       [&](const std::vector<int>& v) { return v.empty(); }),
        result.end());
        return result;
    }
};