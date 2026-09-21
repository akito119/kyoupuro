#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
double pie = M_PI;
//from my library https://github.com/akito119/kyoupuro
template<int mod>
class ModInt{
    int x;

    int addInvElement() const{
        return mod - x;
    }
    int proInvElement() const{
        assert(x != 0);
        return pow(mod - 2).x;
    }
    public:
    ModInt() : x(0) {}
    ModInt(int x) : x((x % mod + mod) % mod)  {}
    ModInt& operator=(int value){
        x = (value % mod + mod) % mod;
        return *this;
    }
    ModInt operator+(const ModInt& other) const{
        return ModInt{x + other.x};
    }
    ModInt operator-(const ModInt& other) const{
        return ModInt{x + other.addInvElement()};
    }
    ModInt operator*(const ModInt& other) const{
        return ModInt{x * other.x};
    }
    ModInt operator/(const ModInt& other) const {
        return ModInt{x * other.proInvElement()};
    }

    ModInt& operator+=(const ModInt& other) {
        *this = *this + other;
        return *this;
    }
    ModInt& operator-=(const ModInt& other) {
        *this = *this - other;
        return *this;
    }
    ModInt& operator*=(const ModInt& other) {
        *this = *this * other;
        return *this;
    }
    ModInt& operator/=(const ModInt& other) {
        *this = *this / other;
        return *this;
    }

    ModInt pow(int exponent) const {
        assert(exponent >= 0);
        ModInt res = 1;
        ModInt base = *this;

        while(exponent > 0) {
            if(exponent & 1) {
                res *= base;
            }
            base *= base;
            exponent >>= 1;
        }

        return res;
    }

    static ModInt combination(int n, int r) {
        assert(0 <= n && n < mod);
        if(r < 0 || r > n) {
            return 0;
        }

        r = min(r, n - r);
        ModInt numerator = 1;
        ModInt denominator = 1;
        for(int i = 1; i <= r; i++) {
            numerator *= n - r + i;
            denominator *= i;
        }

        return numerator / denominator;
    }

    bool operator==(const ModInt& other) const {
        return x == other.x;
    }

    bool operator!=(const ModInt& other) const {
        return x != other.x;
    }
    bool operator>(const ModInt& other) const {
        return x > other.x;
    }
    bool operator<(const ModInt& other) const {
        return x < other.x;
    }
    bool operator>=(const ModInt& other) const {
        return x >= other.x;
    }
    bool operator<=(const ModInt& other) const {
        return x <= other.x;
    }
    friend ostream& operator<<(ostream& os, const ModInt& a) {
        return os << a.x;
    }

    friend istream& operator>>(istream& is, ModInt& a) {
        int value;
        is >> value;
        a.x = (value % mod + mod) % mod;
        return is;
    }
};

using CyclicGroup = ModInt<1000000007>;


lint main() {
    int n,a,b;
    cin>>n>>a>>b;

    CyclicGroup ans = 2;
    ans = ans.pow(n);
    ans -= ans.combination(n,a) + ans.combination(n,b);

    cout<<ans - 1<<endl;
}
