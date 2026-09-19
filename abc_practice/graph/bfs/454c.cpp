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
    set<int> ans = {0};

    vector<vector<int>> g(n,vector<int> {});

    rep(i, m){
        int a,b;
        cin>>a>>b;
        a--;
        b--;
        g[a].push_back(b);
    }

    queue<int> q;
    vector<bool> def(n,false);
    q.push(0);

    while(!q.empty()){
        int pos = q.front();
        q.pop();
        if(def[pos]){
            continue;
        }

        def[pos] = true;
        //cout<<pos<<endl;
        for(auto x:g[pos]){
            //cout<<x<<endl;
                q.push(x);
                ans.insert(x);
        }
    }
    cout<<ans.size()<<endl;
}
