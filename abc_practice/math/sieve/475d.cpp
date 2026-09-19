#include <bits/stdc++.h>
using namespace std;
using lint = int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;

// https://qiita.com/sakofsuken/items/210e082169daba690570

const int bigNum = pow(10, 8);

vector<bool> isp(bigNum + 1, true);

void sieve()
{
    isp[0] = false;
    isp[1] = false;
    for (int i = 2; pow(i, 2) <= bigNum; i++)
    {
        if (isp[i])
            for (int j = 2; i * j <= bigNum; j++)
                isp[i * j] = false;
    }
}

bool solver(string& s, int p){
    string ps = to_string(p);
    if(s.size() != ps.size()){

    }
    rep(i,s.size()){
        rep(j,s.size()){
            if((s[i] == s[j]) != (ps[i] == ps[j])){
                return false;
            }
        }
    }
    return true;
}

lint main() {
    string s;
    cin>>s;

    int ma = 1, mi = 1;
    rep(i,s.size() - 1){
        ma *= 10;
        mi *= 10;
    }
    ma *= 10;
    sieve();
    for(int i =  mi; i < ma;i++){
        if(!isp[i]){
            continue;
        }
        if(!solver(s,i)){
            continue;
        }

        cout<<i<<endl;
        return 0;
    }
    cout<<-1<<endl;
}