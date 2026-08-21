#ifndef GEO_LIBRARY_CPP
#define GEO_LIBRARY_CPP

#include <bits/stdc++.h>
using namespace std;

namespace geo {

using ll = long long;
using ld = long double;
using i128 = __int128_t;

constexpr ld EPS = 1e-12L;
constexpr ld PI = 3.141592653589793238462643383279502884L;

template <class T>
struct Point {
    T x;
    T y;

    Point() : x(0), y(0) {}
    Point(T x_, T y_) : x(x_), y(y_) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }

    Point operator*(T k) const {
        return Point(x * k, y * k);
    }

    Point operator/(T k) const {
        return Point(x / k, y / k);
    }

    Point& operator+=(const Point& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point& operator-=(const Point& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
};

template <class T>
Point<T> operator*(T k, const Point<T>& p) {
    return Point<T>(p.x * k, p.y * k);
}

using PointLL = Point<ll>;
using PointD = Point<ld>;

template <class T>
istream& operator>>(istream& is, Point<T>& p) {
    return is >> p.x >> p.y;
}

template <class T>
ostream& operator<<(ostream& os, const Point<T>& p) {
    return os << p.x << ' ' << p.y;
}

int sgn_i128(i128 x) {
    return (x > 0) - (x < 0);
}

int sgn_ld(ld x) {
    if (x > EPS) return 1;
    if (x < -EPS) return -1;
    return 0;
}

string to_string_i128(i128 x) {
    if (x == 0) return "0";
    bool neg = x < 0;
    if (neg) x = -x;

    string s;
    while (x > 0) {
        s.push_back(char('0' + x % 10));
        x /= 10;
    }
    if (neg) s.push_back('-');
    reverse(s.begin(), s.end());
    return s;
}

i128 dot128(PointLL a, PointLL b) {
    return (i128)a.x * b.x + (i128)a.y * b.y;
}

i128 cross128(PointLL a, PointLL b) {
    return (i128)a.x * b.y - (i128)a.y * b.x;
}

i128 cross128(PointLL a, PointLL b, PointLL c) {
    return cross128(b - a, c - a);
}

i128 norm2_128(PointLL p) {
    return dot128(p, p);
}

i128 dist2_128(PointLL a, PointLL b) {
    return norm2_128(a - b);
}

int ccw(PointLL a, PointLL b, PointLL c) {
    return sgn_i128(cross128(a, b, c));
}

bool on_segment_closed(PointLL a, PointLL b, PointLL p) {
    if (cross128(a, b, p) != 0) return false;
    return min(a.x, b.x) <= p.x && p.x <= max(a.x, b.x)
        && min(a.y, b.y) <= p.y && p.y <= max(a.y, b.y);
}

bool segments_intersect_closed(PointLL a, PointLL b, PointLL c, PointLL d) {
    const int ab_c = ccw(a, b, c);
    const int ab_d = ccw(a, b, d);
    const int cd_a = ccw(c, d, a);
    const int cd_b = ccw(c, d, b);

    if (ab_c == 0 && on_segment_closed(a, b, c)) return true;
    if (ab_d == 0 && on_segment_closed(a, b, d)) return true;
    if (cd_a == 0 && on_segment_closed(c, d, a)) return true;
    if (cd_b == 0 && on_segment_closed(c, d, b)) return true;

    return ab_c * ab_d < 0 && cd_a * cd_b < 0;
}

struct LineLL {
    i128 a;
    i128 b;
    i128 c;
};

LineLL line_through_points(PointLL p, PointLL q) {
    const i128 a = (i128)q.y - p.y;
    const i128 b = (i128)p.x - q.x;
    const i128 c = a * p.x + b * p.y;
    return LineLL{a, b, c};
}

LineLL perpendicular_bisector(PointLL p, PointLL q) {
    return LineLL{
        2 * ((i128)q.x - p.x),
        2 * ((i128)q.y - p.y),
        norm2_128(q) - norm2_128(p),
    };
}

bool lines_parallel(LineLL l1, LineLL l2) {
    return l1.a * l2.b - l1.b * l2.a == 0;
}

bool same_line(LineLL l1, LineLL l2) {
    return lines_parallel(l1, l2)
        && l1.a * l2.c - l1.c * l2.a == 0
        && l1.b * l2.c - l1.c * l2.b == 0;
}

bool lines_have_common_point(LineLL l1, LineLL l2) {
    return !lines_parallel(l1, l2) || same_line(l1, l2);
}

bool concentric_circles_exist_through_pairs(
    PointLL p,
    PointLL q,
    PointLL r,
    PointLL s
) {
    return lines_have_common_point(
        perpendicular_bisector(p, q),
        perpendicular_bisector(r, s)
    );
}

struct CircleLL {
    PointLL c;
    ll r;
};

enum class CircleRelation {
    Same,
    Separate,
    ExternallyTangent,
    IntersectTwoPoints,
    InternallyTangent,
    Contained,
};

CircleRelation circle_relation(CircleLL a, CircleLL b) {
    const i128 d2 = dist2_128(a.c, b.c);
    const i128 sum = (i128)a.r + b.r;
    i128 diff = (i128)a.r - b.r;
    if (diff < 0) diff = -diff;

    if (d2 == 0 && a.r == b.r) return CircleRelation::Same;
    if (d2 > sum * sum) return CircleRelation::Separate;
    if (d2 == sum * sum) return CircleRelation::ExternallyTangent;
    if (d2 < diff * diff) return CircleRelation::Contained;
    if (d2 == diff * diff) return CircleRelation::InternallyTangent;
    return CircleRelation::IntersectTwoPoints;
}

bool circles_have_common_point(CircleLL a, CircleLL b) {
    const CircleRelation rel = circle_relation(a, b);
    return rel != CircleRelation::Separate && rel != CircleRelation::Contained;
}

int circle_common_point_count(CircleLL a, CircleLL b) {
    const CircleRelation rel = circle_relation(a, b);
    if (rel == CircleRelation::Same) return -1;
    if (rel == CircleRelation::Separate || rel == CircleRelation::Contained) return 0;
    if (rel == CircleRelation::ExternallyTangent || rel == CircleRelation::InternallyTangent) return 1;
    return 2;
}

int point_circle_position(PointLL p, CircleLL c) {
    return sgn_i128(dist2_128(p, c.c) - (i128)c.r * c.r);
}

i128 polygon_area2_signed(const vector<PointLL>& p) {
    i128 res = 0;
    const int n = (int)p.size();
    for (int i = 0; i < n; i++) {
        res += cross128(p[i], p[(i + 1) % n]);
    }
    return res;
}

i128 polygon_area2_abs(const vector<PointLL>& p) {
    i128 res = polygon_area2_signed(p);
    if (res < 0) res = -res;
    return res;
}

vector<PointLL> convex_hull(vector<PointLL> p) {
    sort(p.begin(), p.end());
    p.erase(unique(p.begin(), p.end()), p.end());

    if ((int)p.size() <= 1) return p;

    vector<PointLL> lower;
    for (PointLL x : p) {
        while ((int)lower.size() >= 2
            && cross128(lower[(int)lower.size() - 2], lower.back(), x) <= 0) {
            lower.pop_back();
        }
        lower.push_back(x);
    }

    vector<PointLL> upper;
    for (int i = (int)p.size() - 1; i >= 0; i--) {
        PointLL x = p[i];
        while ((int)upper.size() >= 2
            && cross128(upper[(int)upper.size() - 2], upper.back(), x) <= 0) {
            upper.pop_back();
        }
        upper.push_back(x);
    }

    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

enum class PointInPolygon {
    Outside,
    OnBoundary,
    Inside,
};

PointInPolygon point_in_polygon(const vector<PointLL>& poly, PointLL q) {
    bool inside = false;
    const int n = (int)poly.size();

    for (int i = 0; i < n; i++) {
        PointLL a = poly[i];
        PointLL b = poly[(i + 1) % n];

        if (on_segment_closed(a, b, q)) return PointInPolygon::OnBoundary;

        const bool crosses_y = (a.y > q.y) != (b.y > q.y);
        if (!crosses_y) continue;

        const i128 lhs = (i128)(b.x - a.x) * (q.y - a.y);
        const i128 rhs = (i128)(q.x - a.x) * (b.y - a.y);

        if (b.y > a.y) {
            if (lhs > rhs) inside = !inside;
        } else {
            if (lhs < rhs) inside = !inside;
        }
    }

    return inside ? PointInPolygon::Inside : PointInPolygon::Outside;
}

ld dot(PointD a, PointD b) {
    return a.x * b.x + a.y * b.y;
}

ld cross(PointD a, PointD b) {
    return a.x * b.y - a.y * b.x;
}

ld cross(PointD a, PointD b, PointD c) {
    return cross(b - a, c - a);
}

ld norm2(PointD p) {
    return dot(p, p);
}

ld dist(PointD a, PointD b) {
    return sqrtl(norm2(a - b));
}

PointD perp(PointD p) {
    return PointD(-p.y, p.x);
}

PointD unit(PointD p) {
    return p / sqrtl(norm2(p));
}

PointD rotate(PointD p, ld rad) {
    const ld cs = cosl(rad);
    const ld sn = sinl(rad);
    return PointD(p.x * cs - p.y * sn, p.x * sn + p.y * cs);
}

PointD projection_to_line(PointD a, PointD b, PointD p) {
    const PointD ab = b - a;
    return a + ab * (dot(p - a, ab) / norm2(ab));
}

PointD reflection_over_line(PointD a, PointD b, PointD p) {
    const PointD foot = projection_to_line(a, b, p);
    return foot * 2.0L - p;
}

ld distance_point_line(PointD a, PointD b, PointD p) {
    return fabsl(cross(b - a, p - a)) / sqrtl(norm2(b - a));
}

ld distance_point_segment(PointD a, PointD b, PointD p) {
    if (sgn_ld(dot(b - a, p - a)) < 0) return dist(a, p);
    if (sgn_ld(dot(a - b, p - b)) < 0) return dist(b, p);
    return distance_point_line(a, b, p);
}

optional<PointD> line_intersection(PointD a, PointD b, PointD c, PointD d) {
    const PointD ab = b - a;
    const PointD cd = d - c;
    const ld den = cross(ab, cd);
    if (sgn_ld(den) == 0) return nullopt;

    const ld t = cross(c - a, cd) / den;
    return a + ab * t;
}

vector<PointD> circle_line_intersections(PointD center, ld radius, PointD a, PointD b) {
    const PointD ab = b - a;
    const PointD foot = projection_to_line(a, b, center);
    const ld h2 = radius * radius - norm2(foot - center);

    if (h2 < -EPS) return {};
    if (fabsl(h2) <= EPS) return {foot};

    const PointD shift = unit(ab) * sqrtl(max((ld)0, h2));
    return {foot - shift, foot + shift};
}

vector<PointD> circle_circle_intersections(PointD c1, ld r1, PointD c2, ld r2) {
    const PointD v = c2 - c1;
    const ld d2 = norm2(v);
    if (d2 <= EPS * EPS) return {};

    const ld d = sqrtl(d2);
    if (d > r1 + r2 + EPS) return {};
    if (d + min(r1, r2) < max(r1, r2) - EPS) return {};

    const ld x = (d2 + r1 * r1 - r2 * r2) / (2.0L * d);
    const ld h2 = r1 * r1 - x * x;
    if (h2 < -EPS) return {};

    const PointD base = c1 + v * (x / d);
    if (fabsl(h2) <= EPS) return {base};

    const PointD shift = perp(v) * (sqrtl(max((ld)0, h2)) / d);
    return {base - shift, base + shift};
}

}  // namespace geo

#endif  // GEO_LIBRARY_CPP
