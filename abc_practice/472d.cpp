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
    int h,w,k;
    cin>>h>>w>>k;
    vector<string> s(h);
    rep(i,h) cin>>s[i];
    vector<int> x(h),y(w);
    rep(i,h){
        rep(j,w){
            x[i] = s[i][j] == '#'? 1:max(x[i],(int)0);
            y[j] = s[i][j] == '#'? 1:max(y[j],(int) 0);
        }
    }

    set<pair<int,int>> not_safe;
    rep(i,h){
            rep(j,w){
                if(y[j] == 1 || x[i] == 1){
                    not_safe.insert({i,j});
                }
            }
    }

    vector<pair<int,int>> safe(0);
    rep(i,h){
        rep(j,w){
            if(!not_safe.count({i,j})){
                safe.push_back({i,j});
            }
        }
    }

    map<pair<int,int>,vector<pair<int,int>>> g;
    rep(i, h){
        rep(j, w){
            g[{i, j}] = {};
        }
    }

    rep(i,h){
        rep(j,w){
            if(s[i][j] == '#'){
                continue;
            }
            if(i + 1 < h){
                if(s[i + 1][j] == '.'){
                    g[{i,j}].push_back({i+1,j});
                    g[{i + 1,j}].push_back({i,j});
                }
            }
            if(j + 1 < w){
                if(s[i][j+1] == '.'){
                    g[{i,j}].push_back({i,j + 1});
                    g[{i,j + 1}].push_back({i,j});
                }
            }
        }
    }

    queue<pair<int,int>> q;
    vector<vector<int>> cur(h,vector<int> (w)),def(h,vector<int> (w));

    for(auto x:safe){
        q.push(x);
    }
    rep(i,h){
        rep(j,w){
            cur[i][j] = LLONG_MAX;
            def[i][j] = 0;
        }
    }
    for(auto x:safe){
        int i = x.first,j = x.second;
        cur[i][j] = 0;
    }

    while(!q.empty()){
        pair<int,int> res = q.front();
        q.pop();

        int x = res.first,y = res.second;
        if(def[x][y] == 1){
            continue;
        }
        def[x][y] = 1;
        for(auto G:g[res]){
            if(cur[G.first][G.second] > cur[x][y] + 1){
                cur[G.first][G.second] = cur[x][y] + 1;
                q.push(G);
            }
        }
    }

    int ans = 0;
    rep(i,h){
        rep(j,w){
            //cout<<i<<" "<<j<<" "<<cur[i][j]<<endl;
            if(cur[i][j] <= k){
                ans ++;
            }
        }
    }

    cout<<ans<<endl;
}
