#include <./bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
int prime = 998244353;
double pie = M_PI;

//各回の選択が2択なのでDPっぽい
//解説読んでも理解できなかった。じあー
lint main(){
    int n;
    cin>>n;
    int k;
    cin>>k;
    vector<int> l(0),r(0);
    for(int i= 0;i<n;i++){
        int a ;
        cin>>a;
        if(a<0){
            l.push_back(a);
        }
        else{
            r.push_back(a);
        }
    }
    reverse(l.begin(),l.end());
    int use_left = 0, use_right = 0, now = 0;

}
