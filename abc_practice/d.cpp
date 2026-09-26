#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double

#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;
int inf = LLONG_MAX;


lint main() {
    int n, q;
    cin>>n>>q;
    int count = 0;
    map<int,char> C = {{0, 'a'}};
    vector<pair<int,bool>> t(n, {0,false});
    set<int> a;
    rep(i, q){
        int ty;
        cin>>ty;
        if(ty == 1){
            int x;
            cin>>x;
            x--;

            if(t[x].second){
                a.insert(x);
                t[x].second = false;
            }
            else{
                if(a.count(x)){
                    t[x].second = true;
                    continue;
                }
                t[x] = {count, true};
            }

        }
        else{
            char c;
            cin>>c;
            count++;
            C[count] = c;
            a = {};
        }
    }
    rep(i, n){
        if(a.count(i) || t[i].second){
            cout<<C[t[i].first];
        }
        else{
            cout<<C[count];
        }
    }

    cout<<endl;
}