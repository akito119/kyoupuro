#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;


lint main(){
    double n,k;
    cin>>n>>k;
    double ans = 0;
    for(int i = 1;i<=n;i++){
        double num = 1;
        double ten = i;
        while(ten<k){
            num *=2;
            ten *= 2;
        }
        ans += 1/(n*num);
        //cout<<i*num<<endl;
    }
    cout<<fixed<<setprecision(30)<<ans<<endl;
}
