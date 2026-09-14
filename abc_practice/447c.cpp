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
    string s,t;
    cin>>s>>t;
    map<int,char> cs,ct;
    vector<int> ns(0),nt(0);

    int c = 0;
    int count = 0;
  
    for(auto x : s){
        if(x == 'A'){
            count ++;
        }
        else{
            ns.push_back(count);
            count = 0;
            cs[c] = x;
            c ++;
        }
    }
    ns.push_back(count);
    count = 0;
    c = 0;

    for(auto x : t){
        //cout<<x<<endl;
        if(x == 'A'){
            count ++;
        }
        else{
            nt.push_back(count);
            //cout<<x<<" "<<count<<endl;
            count = 0;
            ct[c] = x;
            c ++;
        }
        //cout<<count<<endl;
    }
    nt.push_back(count);

    if(ct.size() != cs.size()){
        cout<<-1<<endl;
        return 0;
    }

    rep(i,ct.size()){
        if(ct[i] != cs[i]){
            cout<<-1<<endl;
            return 0;
        }
    }


    int ans = 0;
    rep(i,nt.size()){
        //cout<<nt[i]<<" "<<ns[i]<<endl;
        ans += abs(nt[i] - ns[i]);
    }
    cout<<ans<<endl;
}
