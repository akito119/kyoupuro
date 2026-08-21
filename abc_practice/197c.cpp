#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;


lint main(){
    int n;
    cin>>n;
    vector<bitset<31>> a(n);
    for(int i= 0;i<n;i++){
        int b;
        cin>>b;
        bitset<31> c(b);
        a[i] = c;
    }
    for(int i = 0;i<pow(2,n-1);i++){
        bidset<n-1> status(i);
        
    }
}
