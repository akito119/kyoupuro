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
class decsort {
public:
    struct point {
        long long x, y;
    };

private:
    vector<point> upper, lower;

    static __int128 cross(const point& a, const point& b) {
        return (__int128)a.x * b.y - (__int128)a.y * b.x;
    }

    static unsigned __int128 norm2(const point& p) {
        const unsigned __int128 x = p.x < 0 ? -(__int128)p.x : p.x;
        const unsigned __int128 y = p.y < 0 ? -(__int128)p.y : p.y;
        return x * x + y * y;
    }

    static bool angle_less(const point& a, const point& b) {
        const __int128 c = cross(a, b);
        if (c != 0) return c > 0;
        return norm2(a) < norm2(b);
    }

public:
    decsort() = default;

    void add(long long x, long long y) {
        // The zero vector is placed first, as if its angle were zero.
        ((y > 0 || (y == 0 && x >= 0)) ? upper : lower).push_back({x, y});
    }

    void add(point p) { add(p.x, p.y); }

    // Equal-angle vectors are ordered by distance from the origin.
    // Time: O(n log n); extra space for the returned vector: O(n).
    std::vector<point> sort() {
        std::sort(upper.begin(), upper.end(), angle_less);
        std::sort(lower.begin(), lower.end(), angle_less);
        std::vector<point> result;
        result.reserve(upper.size() + lower.size());
        result.insert(result.end(), upper.begin(), upper.end());
        result.insert(result.end(), lower.begin(), lower.end());
        return result;
    }
};
