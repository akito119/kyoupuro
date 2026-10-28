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
    int n, k, t;
    cin>>n>>k>>t;
    vector<int> a(n);
    rep(i, n){
        cin>>a[i];
    }

    int end = k;

    int ans = 1;

    a.push_back(t);
    sort(a.begin(), a.end());
    if(a[0] == 0){
        ans = 0;
        reverse(a.begin(), a.end());
        a.pop_back();
        reverse(a.begin(), a.end());
    }
    rep(i,a.size() ){
        if(a[i] < end){
            end = a[i] + k;
        }
        else{
            ans += (a[i] - end) / k;
            if((a[i] - end) % k > 0){
                ans++;
            }
            end = a[i] + k;
        }


        //cout<<ans<<" "<<end<<endl;
        if(a[i] == t){
            break;
        }
    }

    cout<<ans<<endl;

}
