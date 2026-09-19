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
    int l = 0,r = n;

    for(int i= 0;i<20;i++){
        int c = (l+r)/2;
        cout<<"? "<<c<<endl;
        int s;
        cin>>s;
        if(s == 1){
            r = c;
        }
        else{
            l = c;
        }
    }
    cout<<"! "<<l<<endl;
    return 0;
}