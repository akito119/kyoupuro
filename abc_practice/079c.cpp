#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;
int inf = LLONG_MAX;

lint main(){
    string s;
    cin>>s;
    int a = s[0]-'0',b = s[1]-'0',c = s[2]-'0', d = s[3]-'0';
    map<char,char> o{{0,'+'},{1,'-'}};
    map<char,int> p{{0,1},{1,-1}};
    for(int i= 1;i<=8;i++){
        bitset<3> bs(i);
        int ans = a+p[bs[2]]*b+p[bs[1]]*c+p[bs[0]]*d;
        if(ans == 7){
            cout<<a<<o[bs[2]]<<b<<o[bs[1]]<<c<<o[bs[0]]<<d<<"=7"<<endl;
            return 0;
        }
    }
}