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
//studied form https://lets-csharp.com/sort-using-cross-products/

// Sort vectors counterclockwise from the positive x-axis, without atan2.
struct point {
    long long x, y;

    bool operator<(const point& other) const {
        const bool this_upper = y > 0 || (y == 0 && x >= 0);
        const bool other_upper = other.y > 0 || (other.y == 0 && other.x >= 0);
        if (this_upper != other_upper) return this_upper;

        const __int128 c = (__int128)x * other.y - (__int128)y * other.x;
        if (c != 0) return c > 0;

        const unsigned __int128 ax = x < 0 ? -(__int128)x : x;
        const unsigned __int128 ay = y < 0 ? -(__int128)y : y;
        const unsigned __int128 bx = other.x < 0 ? -(__int128)other.x : other.x;
        const unsigned __int128 by = other.y < 0 ? -(__int128)other.y : other.y;
        return ax * ax + ay * ay < bx * bx + by * by;
    }

    point com(int k) {
        point r = {k * x, k * y};
        return r;
    }
};

struct decsort {
    vector<point> upper, lower;

    decsort() = default;

    void add(long long x, long long y) {
        // The zero vector is placed first, as if its angle were zero.
        ((y > 0 || (y == 0 && x >= 0)) ? upper : lower).push_back({x, y});
    }

    void add(point p) { add(p.x, p.y); }

    // Equal-angle vectors are ordered by distance from the origin.
    // Time: O(n log n); extra space for the returned vector: O(n).
    std::vector<point> sort() {
        std::sort(upper.begin(), upper.end());
        std::sort(lower.begin(), lower.end());
        std::vector<point> result;
        result.reserve(upper.size() + lower.size());
        result.insert(result.end(), upper.begin(), upper.end());
        result.insert(result.end(), lower.begin(), lower.end());
        return result;
    }
};



int solve (point s, auto& a){
    auto it = upper_bound(a.begin(), a.end(), s);
    return it - a.begin();
}
lint main () {
    int t;
    cin>>t;
    rep(T, t)
    {
        int n;
        cin>>n;
        decsort dec;
        rep(i, n){
            int x, y;
            cin>>x>>y;
            dec.add(x, y);
        }
        dec.sort();


        int ans = 0;
        rep(i, dec.upper.size()){
            ans = max((int)dec.upper.size() - i + solve(dec.upper[i].com(- 1), dec.lower), ans);
        }
        rep(i, dec.lower.size()) {
            ans = max((int)dec.lower.size() - i + solve(dec.lower[i].com(- 1), dec.upper), ans);
        }

        cout<<ans<<endl;
    }
}