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
    int n;
    cin>>n;
    vector<int> a(n);
    rep(i, n){
        cin>>a[i];
    }

    int ans = 0;
    int count = 1;
    int tmp = a[0];

    for(int i = 1;i< n;i++){
        if(tmp > a[i]){
            ans = max(ans, count);
            count = 1;
            tmp = a[i];
            continue;
        }
        else{
            count++;
            tmp = a[i];
        }
    }
    ans = max(ans,count);
    cout<<ans<<endl;
}
