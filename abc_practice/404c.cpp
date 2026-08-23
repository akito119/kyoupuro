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
    int n,m;
    cin>>n>>m;
    vector<vector<int>> g(n,vector<int> (0));
    rep(i, m){
        int a, b;
        cin>>a>>b;
        a--, b--;
        g[a].push_back(b);
        g[b].push_back(a);
    }

    bool flag = true;
    rep(i,n){
        if(g[i].size() != 2){
            flag = false;
        }
    }

    int num = 1;
    int now = 0;
    int prev = 0;
    while(true){
        int tmp = 0;
        for(auto x:g[now]){
            if(prev != x){
                tmp = x;
            }
        }
        prev = now;
        now = tmp;
        num ++;
        //cout<<now<<endl;
        if(now == 0){
            break;
        }
        if(num > n){
            break;
        }
    }
    if(num != n+1){
        flag = false;
    }
    //cout<<num<<endl;
    string ans = flag ? "Yes" : "No";
    cout<<ans<<endl;
}