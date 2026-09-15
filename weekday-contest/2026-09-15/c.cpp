#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;


lint main(){
    int n,m,s;
    cin>>m>>n>>s;
    int ans = 0;
    rep(i, m){
        char c;
        int a;
        cin>>c>>a;
        if(c == 'L'){
            a -= s;
            if(a <= 0){
                s = abs(a);
            }
            else{
            int flip = a/n;
            ans += flip + 1;

            if(flip % 2 == 1){
                s = n - (a % n);
            }
            else{
                s = a % n;
            }
        }
        cout<<s<<" "<<ans<<endl;
        }
        else{
            a -= n - s;
            cout<<a<<endl;
            if(a <= 0){
                s = n + a;
                continue;
            }
            else{
            int flip = a/n;
            ans += flip + 1;
            if(flip % 2 != 1){
                s = n - (a % n);
            }
            else{
                s = a % n;
            }
        }
cout<<s<<" "<<ans<<endl;
        }
    }
    cout<<s<<" "<<ans<<endl;
}