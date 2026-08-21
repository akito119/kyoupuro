#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;


lint main(){
    int n,m;
    cin>>n>>m;
    vector<int> a(n);
    for(int i=0;i<n;i++){
        cin>>a[i];
    }
    map<int,int> x;
    set<int> ans;
    for(int i= 0;i<n;i++){
        if(!x.count(a[i])){
            x[a[i]] = -1;
        }
    }
    for(int i = 0;i<n;i++){
        if(abs(x[a[i]]-i)>m){
            ans.insert(a[i]);
        }
        else{
            x[a[i]] = i;
        }
    }
    for(int i = 0;i<n;i++){
        if(x.count(i)){
            if(abs(n-x[i])>m){
                ans.insert(i);
            }
        }
    }
    for(int i=0;i<n+1;i++){
        if(ans.count(i)||!x.count(i)){
            cout<<i<<endl;
            return 0;
        }
    }
}