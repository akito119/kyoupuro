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

/*int score (){
   スコア決定関数 
}*/


lint main(){
   auto start_time = chrono::steady_clock::now(); 
   //初期解を作成


    while(true){
         double elapsed = chrono::duration<double>(
            chrono::steady_clock::now() - start_time
        ).count();

        if (elapsed >= 1.8) break;
        //改善関数の作成
    }

    //出力
}
