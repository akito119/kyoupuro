#include <bits/stdc++.h>
using namespace std;
using lint=int;
#define int long long
#define int128 __int128_t
#define double long double
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
int prime = 998244353;
double pie = M_PI;


const unsigned long long seed_value =
    chrono::steady_clock::now().time_since_epoch().count();

mt19937_64 rng(seed_value);

// [l, r] の整数
template<class T>
T ranint(T l, T r) {
    static_assert(is_integral_v<T>);
    return uniform_int_distribution<T>(l, r)(rng);
}

int score (vector<int>& c, vector<int>& ans, vector<vector<int>>& s){
    int score = 0;
    vector<int> last(26, -1);
    rep(i, ans.size()){
        score += s[i][ans[i] - 1];
        last[ans[i] - 1] = i;
        rep(j, last.size()){
            score -= c[j] * (i - last[j]);
        }
    }

    return score;
}


lint main(){
   auto start_time = chrono::steady_clock::now(); 
   int d;
   cin>>d;
    vector<int> c(26);
    rep(i, 26){
        cin>>c[i];
    }

    vector<int> last(26);
   vector<vector<int>> s(d, vector<int> (26));
    rep(i, d){
        rep(j, 26){
            cin>>s[i][j];
        }
    }

    vector<int> ans(d);

    rep(i, d) {
        int index = 0;
        for(int j = 0; j< 26; j++){
            if(s[i][index] - c[j] * (i + 1 - last[j]) < s[i][j] - c[index] * (i + 1 - last[index])){
                index = j;
            }
        }

        ans[i] = index + 1;
        last[index] = i + 1;
    }


    //貪欲解正の得点OK

    int sc = score(c, ans, s);//初期解


    while(true){
         double elapsed = chrono::duration<double>(
            chrono::steady_clock::now() - start_time
        ).count();

        if (elapsed >= 1.8) break;


        int c_index = ranint((int) 0, d - 1);
        int c_num = ranint(1, 26);
        int tmp = ans[c_index];

        ans[c_index] = c_num;

        if(sc > score(c, ans, s)){
            ans[c_index] = tmp;
        }
        else{
            sc = score(c, ans, s);
        }
    }

    rep(i, d){
        cout<<ans[i]<<endl;
    }
}
