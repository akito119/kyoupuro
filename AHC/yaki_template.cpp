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

    const double time_limit = 1.8;

    // スコアの変化量に合わせて調整
    const double start_temperature = 1000.0;
    const double end_temperature = 1.0;

    uniform_real_distribution<double> randdouble(0.0, 1.0);

    // 初期解を作成

    // int current_score = score();

    // 焼きなましでは現在解が悪化することがあるので、
    // 必要なら最良解も別に保存しておく
    // int best_score = current_score;

    while(true){
        double elapsed = chrono::duration<double>(
            chrono::steady_clock::now() - start_time
        ).count();

        if (elapsed >= time_limit) break;

        double progress = elapsed / time_limit;

        double temperature = start_temperature * pow(
            end_temperature / start_temperature,
            progress
        );

        // 変更箇所を決める

        // 変更を適用する

        // int new_score = score();
        // int diff = new_score - current_score;

        /*
        if (
            diff >= 0 ||
            randdouble(rng) < exp((double)diff / temperature)
        ) {
            // 変更を採用
            // current_score = new_score;

            // 最良解なら保存
        } else {
            // 変更を元に戻す
        }
        */
    }

    // 最良解を出力
}
