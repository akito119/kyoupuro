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

/*int evaluate (){
   途中状態の評価関数
}*/


lint main(){
    const int beam_width = 100;

    // 初期状態を作成

    /*
    vector<状態の型> beam;
    beam.push_back(初期状態);
    */

    // 必要な手数だけ繰り返す
    /*
    rep(turn, max_turn){
        vector<状態の型> next_beam;

        for (auto current_state : beam) {
            // current_stateから可能な操作を列挙

            for (auto action : actions) {
                auto next_state = current_state;

                // next_stateにactionを適用

                // next_stateの評価値を計算

                next_beam.push_back(next_state);
            }
        }

        // 評価値が大きい順に並べる
        sort(
            next_beam.begin(),
            next_beam.end(),
            [&](const auto& a, const auto& b){
                return aの評価値 > bの評価値;
            }
        );

        // 上位beam_width個だけ残す
        if ((int)next_beam.size() > beam_width) {
            next_beam.resize(beam_width);
        }

        beam = move(next_beam);
    }
    */

    // beamの中で最も良い完成解を出力
}