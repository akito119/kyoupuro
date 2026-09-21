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
class CyclicGroup{
    int x;

    int addInvElement() const{
        return prime - x;
    }
    int proInvElement() const{
        assert(x != 0);
        return pow(prime - 2).x;
    }
    public:
    CyclicGroup() : x(0) {}
    CyclicGroup(int x) : x((x % prime + prime) % prime)  {}
    CyclicGroup& operator=(int value){
        x = (value % prime + prime) % prime;
        return *this;
    }
    CyclicGroup operator+(const CyclicGroup& other) const{
        return CyclicGroup{x + other.x};
    }
    CyclicGroup operator-(const CyclicGroup& other) const{
        return CyclicGroup{x + other.addInvElement()};
    }
    CyclicGroup operator*(const CyclicGroup& other) const{
        return CyclicGroup{x * other.x};
    }
    CyclicGroup operator/(const CyclicGroup& other) const {
        return CyclicGroup{x * other.proInvElement()};
    }

    CyclicGroup& operator+=(const CyclicGroup& other) {
        *this = *this + other;
        return *this;
    }
    CyclicGroup& operator-=(const CyclicGroup& other) {
        *this = *this - other;
        return *this;
    }
    CyclicGroup& operator*=(const CyclicGroup& other) {
        *this = *this * other;
        return *this;
    }
    CyclicGroup& operator/=(const CyclicGroup& other) {
        *this = *this / other;
        return *this;
    }

    // Binary exponentiation: returns (*this)^exponent.
    CyclicGroup pow(int exponent) const {
        assert(exponent >= 0);
        CyclicGroup res = 1;
        CyclicGroup base = *this;

        while(exponent > 0) {
            if(exponent & 1) {
                res *= base;
            }
            base *= base;
            exponent >>= 1;
        }

        return res;
    }

    // Returns nCr modulo prime. This implementation requires 0 <= n < prime.
    static CyclicGroup combination(int n, int r) {
        assert(0 <= n && n < prime);
        if(r < 0 || r > n) {
            return 0;
        }

        r = min(r, n - r);
        CyclicGroup numerator = 1;
        CyclicGroup denominator = 1;
        for(int i = 1; i <= r; i++) {
            numerator *= n - r + i;
            denominator *= i;
        }

        return numerator / denominator;
    }

    bool operator==(const CyclicGroup& other) const {
        return x == other.x;
    }

    bool operator!=(const CyclicGroup& other) const {
        return x != other.x;
    }
    bool operator>(const CyclicGroup& other) const {
        return x > other.x;
    }
    bool operator<(const CyclicGroup& other) const {
        return x < other.x;
    }
    bool operator>=(const CyclicGroup& other) const {
        return x >= other.x;
    }
    bool operator<=(const CyclicGroup& other) const {
        return x <= other.x;
    }
    friend ostream& operator<<(ostream& os, const CyclicGroup& a) {
        return os << a.x;
    }

    friend istream& operator>>(istream& is, CyclicGroup& a) {
        int value;
        is >> value;
        a.x = (value % prime + prime) % prime;
        return is;
    }
};
