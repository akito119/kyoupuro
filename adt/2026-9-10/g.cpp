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
    int h,w,s,c,n;
    cin>>h>>w>>s>>c>>n;
    map<int,vector<int>> x,y;
    rep(i,n){
        int a,b;
        cin>>a>>b;
        if(!x.count(a)){
            x[a] = {};
        }
        x[a].push_back(b);
        if(!y.count(b)){
            y[b] = {};
        }
        y[b].push_back(a);
    }

    for(auto a:x){
        sort(a.second.begin(),a.second.end());
    }
    for(auto a:y){
        sort(a.second.begin(),a.second.end());
    }

    int q;
    cin>>q;
    rep(i,q){
        char a;
        int b;
        cin>>a>>b;
        int wl = LLONG_MAX,wl2 = 0,wr = LLONG_MAX,wr2 = 0;
        if(x.count(s)){
        auto it = upper_bound(x[s].begin(),x[s].end(),c);
        wl = *it,wl2 = it!= x[s].begin() ? *prev(it,1) : wl ;
        }
        if(y.count(c)){
        auto it2 = upper_bound(y[c].begin(),y[c].end(),s);
        wr = *it2, wr2 = it2 == y[c].begin() ?*prev(it2,1):wr;
        cout<<wr<<" "<<wr2<<endl;
        }
        //LorRのときcいじる壁はx参照
        if(a == 'L'){
            if(c - b <= wl2){
                c = wl2 + 1;
            }
            else if(c - b <= 0){
                c = 1;
            }
            else{
                c -= b;
            }
        }
        if(a == 'R'){
            if(c + b >= wl){
                c = wl - 1;
            }
            else if(c + b >= w){
                c = w;
            }
            else{
                c += b;
            }
        }
        if(a == 'U'){
            cout<<wl2<<endl;
            if(s - b <= wr2){
                 s = wr2 + 1;
            }
            else if(s - b <= 0){
                s = 1;
            }
            else{
                s -= b;
            }
        }
        if(a == 'D'){
            if( s + b >= wr){
                s = wr -1;
            }
            else if(a + b >= h){
                s = h;
            }
            else{
                s += b;
            }
        }
        cout<<s<<" "<<c<<endl;

    }
}
