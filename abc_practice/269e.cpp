#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;
//なぜか通らないので後日AIデバッグ

lint main(){
    int n;
    cin>>n;
    int l = 1,r = n;
    while(l<r){
        int c = (l+r)/2;
        cout<<"? "<<1<<" "<<n<<" "<<l<<" "<<c<<endl;
        int t;
        cin>>t;
        if(t==c){
            l = c+1;
        }
        else{
            r = c;
        }
    }
    int ans = l;
    l = 1, r = n;
    while(l<r){
        int c = (l+r)/2;
        cout<<"? "<<l<<" "<<c<<" "<<1<<" "<<n<<endl;
        int t;
        cin>>t;
        if(t==c){
            l = c+1;
        }
        else{
            r = c;
        }
    }
    cout<<"! "<<l<<" "<<ans<<endl;
}