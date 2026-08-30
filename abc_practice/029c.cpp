#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;

void dfs(string s, int n){
    if(s.size() == n){
        cout<<s<<endl;
        return;
    }

    for(char x:{'a','b','c'}){
        dfs(s+x,n);
    }

    return;
}

lint main(){
   int n;
   cin>>n;
    dfs("",n);
}