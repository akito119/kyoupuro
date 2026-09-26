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

// chars の中から1文字
char ranchar(
    string_view chars = "abcdefghijklmnopqrstuvwxyz"
) {
    return chars[ranint<size_t>(0, chars.size() - 1)];
}

// 指定した長さの文字列
string ranstring(
    int len,
    string_view chars = "abcdefghijklmnopqrstuvwxyz"
) {
    string result;
    result.reserve(len);

    for (int i = 0; i < len; ++i) {
        result += ranchar(chars);
    }

    return result;
}



void solver(){
    //解法を入力
}

void tester(){
    //愚直解を書く
}

string generate_input(){
    //入力を書く
}


vector<string> tokenize(const string& output) {
    istringstream stream(output);

    vector<string> tokens;
    string token;

    while (stream >> token) {
        tokens.push_back(token);
    }

    return tokens;
}
string run_program(
    void (*program)(),
    const string& input
) {
    istringstream fake_input(input);
    ostringstream fake_output;

    streambuf* old_cin = cin.rdbuf(fake_input.rdbuf());
    streambuf* old_cout = cout.rdbuf(fake_output.rdbuf());

    cin.clear();
    cout.clear();

    program();

    cout.flush();

    cin.rdbuf(old_cin);
    cout.rdbuf(old_cout);

    cin.clear();
    cout.clear();

    return fake_output.str();
}

lint main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    constexpr int TEST_COUNT = 100000;

    for (int test = 1; test <= TEST_COUNT; ++test) {
        string input = generate_input();

        string solver_output = run_program(solver, input);
        string tester_output = run_program(tester, input);

        if (tokenize(solver_output) != tokenize(tester_output)) {
            cerr << "Wrong answer!\n";
            cerr << "test: " << test << '\n';
            cerr << "seed: " << seed_value << "\n\n";

            cerr << "input:\n";
            cerr << input << '\n';

            cerr << "solver output:\n";
            cerr << solver_output << '\n';

            cerr << "expected output:\n";
            cerr << tester_output << '\n';

            return 0;
        }
    }

    cerr << "All tests passed!\n";
}
