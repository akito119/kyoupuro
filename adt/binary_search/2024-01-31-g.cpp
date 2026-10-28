#include <bits/stdc++.h>
using namespace std;
using lint = int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;


lint main() {
    int n;
    cin>>n;
    map<int,vector<int>> a;
    rep(i, n){
        int num;
        cin>>num;
        if(a.count(num)){
            a[num].push_back(i);
        }
        else{
            a[num] = {i};
        }
    }

    int q;
    cin>>q;
    rep(i, q){
        int l, r, x;
        cin>>l>>r>>x;
        l--, r--;
        auto left = lower_bound(a[x].begin(), a[x].end(), l);
        auto right = upper_bound(a[x].begin(), a[x].end(), r);

        int l_num = left - a[x].begin();
        int r_num = right - a[x].begin();
        cout<<r_num - l_num<<endl;
    }
}