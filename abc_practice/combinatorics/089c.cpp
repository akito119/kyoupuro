#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;
vector<char> top = {'M','A','R','C','H'};

lint main(){
    int n;
    cin>>n;
    map<char,int> a;
    for(int i = 0; i<5;i++){
        a[top[i]] = 0;
    }
    for(int i= 0;i<n;i++){
        string s;
        cin>>s;
        for(int i= 0;i<5; i++){
            if(s[0] == top[i]){
                 a[top[i]] ++;
            }
        }
    }
    int ans = 0;
    ans += a['M']*a['A']*a['R'];
    ans += a['M']*a['A']*a['C'];
    ans += a['M']*a['A']*a['H'];
    ans += a['M']*a['C']*a['R'];
    ans += a['M']*a['C']*a['H'];
    ans += a['M']*a['R']*a['H'];
    ans += a['A']*a['R']*a['C'];
    ans += a['A']*a['R']*a['H'];
    ans += a['A']*a['H']*a['C'];
    ans += a['R']*a['C']*a['H'];
    cout<<ans<<endl;
}