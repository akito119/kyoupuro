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
        int res = 1;
        int n = prime - 2;
        int a = x;

        while(n > 0){
            if(n & 1) {
                res = res * a % prime;
            }

            a = a * a % prime;
            n >>= 1;
        }

        return res;
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