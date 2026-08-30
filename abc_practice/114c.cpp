#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;
   int ans = 0;
void dfs(string s, int n){
    if(s.size() == 10){
        int num = stoll(s);
        bool f3 = false, f5 = false, f7 = false, f0 = false, f0p = true, fn = false;
        if(num <= n){
            fn = true;
        }
        while(num != 0){
            if(num % 10 == 3){
                f3 = true;
                if(f0){
                    f0p = false;
                }
            }
            if(num % 10 == 5){
                f5 = true;
                if(f0){
                    f0p = false;
                }
            }
            if(num % 10 == 7){
                f7 = true;
                if(f0){
                    f0p = false;
                }
            }

            if(num % 10 == 0){
                f0 = true;
            }

            num /= 10;
        }

        if(f3 && f5 && f7 && f0p &&fn){
                ans++;
        }

        return;
    }

    for(auto x:{'3', '5', '7', '0'}){
        dfs(s+x, n);
    }
    return;
}

lint main(){
   int n;
   cin>>n;
   dfs("",n);
   cout<<ans<<endl;
}#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;
int ans = 0;

void dfs(auto& b, int128 c, int index, int n, int x){
    if(index == n){
        if(c ==x){
            ans ++;
        }
        return;
    }

    for(auto y: b[index]){
        dfs(b, c*y, index + 1, n, x);
    }
    return;
}

lint main(){
   int n,x;
   cin>>n>>x;
    vector<vector<int>> b(n,vector<int> (0));
   rep(i, n){
        int l;
        cin>>l;
        rep(j,l){
            int a;
            cin>>a;
            b[i].push_back(a);
        }
   }
   dfs(b,(int128) 1, 0, n, x);
   cout<<ans<<endl;
}