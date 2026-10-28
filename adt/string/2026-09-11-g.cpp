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
    string s;
    cin>>n>>s;
    stack<int> l;
    map<int,int> skips;
    rep(i,n){
        if(!l.empty()){
            if(s[i] == ')'){
                skips[l.top()] = i;
                l.pop();
            }
        }

        if(s[i] == '('){
            l.push(i);
        }
    }
    int skip = -1;
    rep(i,n){
        if(i <= skip){
            continue;
        }

        if(skips.count(i)){
            skip = skips[i];
            continue;
        }

        cout<<s[i];
    }
    cout<<endl;
}
