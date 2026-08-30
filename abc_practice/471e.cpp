#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;

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
    int num(){
        return x;
    }
    static CyclicGroup kaijou(CyclicGroup a, CyclicGroup b){
        assert (a >= b);
        if(b <= 0){
            return 1;
        }

        return kaijou(a-1, b-1) * a;
    }
};

#define cy CyclicGroup

lint main(){
    int n,k;
    cin>>n>>k;
    cy cy_n = n, cy_k = k;
    vector<cy> a(n);
    rep(i,n){
        int tmp;
        cin>>tmp;
        a[i] = tmp;
    }
    cy sum = 0;
    rep(i,n) sum += a[i];
    cy ans = 0;
    for(int i= 0;i<n;i++){
        ans += a[i] * a[i];
    }
    ans *= cy::kaijou(cy_n-1, cy_k-1)/cy::kaijou(cy_k-1,cy_k-1);
    if(k == 1){
        cout<<ans.num()<<endl;
        return 0;
    }
    cy c = cy::kaijou(cy_n-2,cy_k-2)/cy::kaijou(cy_k-2,cy_k-2);
    for(int i= 0;i<n;i++){
        ans += c * (a[i] * (sum - a[i]));
    }
    cout<<ans.num()<<endl;
}
