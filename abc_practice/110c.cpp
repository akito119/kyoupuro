#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;


lint main(){
    string s,t;
    cin>>s>>t;
    map<char,int> a,b;
    vector<vector<int>> c(0,vector<int> (0)),d(0, vector<int> (0));
    int pt = 0;
    for(int i= 0;i<s.size();i++){
        if(a.count(s[i])){
            c[a[s[i]]].push_back(i);
        }
        else{
            a[s[i]] = pt;
            pt++;
            c.push_back({i});
        }
    }
    pt = 0;
    for(int i=0;i<t.size();i++){
        if(b.count(t[i])){
            d[b[t[i]]].push_back(i);
        }
        else{
            b[t[i]] = pt;
            pt ++;
            d.push_back({i});
        }
    }
    if(c.size()!= d.size()){
        //cout<<1<<endl;
        cout<<"No"<<endl;
        return 0;
    }

    for(int i= 0;i<c.size();i++){
        if(c[i].size()!= d[i].size()){
            cout<<"No"<<endl;
            return 0;
        }
        for(int j = 0;j<c[i].size();j++){
            if(c[i][j]!=d[i][j]){
                //cout<<"suc"<<c[i][j]<<" "<<d[i][j]<<endl;
                cout<<"No"<<endl;
                return 0;
            }
        }
    }
    cout<<"Yes"<<endl;
}
