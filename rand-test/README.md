# ランダムテスター入力生成リファレンス

`template.cpp` の `generate_input()` で、1テストケース分の入力を文字列として生成する。
生成された同じ入力が `solver()` と `tester()` の両方へ渡される。

## 基本形

```cpp
string generate_input() {
    int n = ranint(1LL, 10LL);

    ostringstream input;
    input << n << '\n';

    for (int i = 0; i < n; ++i) {
        int a = ranint(-100LL, 100LL);
        input << a << (i + 1 == n ? '\n' : ' ');
    }

    return input.str();
}
```

`generate_input()` は、問題の入力形式と同じ文字列を必ず `return` する。
空白や改行は通常の標準入力と同様に扱われるが、デバッグ時に読みやすいよう問題文の形式に合わせるとよい。

## 乱数生成関数

### 整数 `ranint(l, r)`

閉区間 `[l, r]` から整数を1つ生成する。両端を含む。

```cpp
int x = ranint(1LL, 100LL);       // 1以上100以下
int y = ranint(-10LL, 10LL);      // -10以上10以下
size_t i = ranint<size_t>(0, 9);  // size_tを明示する例
```

- `l <= r` にする。
- 引数には同じ整数型を渡す。テンプレートでは `int` が `long long` に置換されるため、整数リテラルには `LL` を付けると型が揃いやすい。
- 問題の制約全体を毎回使う必要はない。愚直解が間に合う小さな範囲に抑える。

### 文字 `ranchar(chars)`

`chars` に含まれる文字から1文字を生成する。

```cpp
char lower = ranchar();          // 'a' ～ 'z'
char bit = ranchar("01");       // '0' または '1'
char c = ranchar("abcXYZ");
```

引数を省略した場合は小文字英字が使われる。空文字列は渡さない。

### 文字列 `ranstring(len, chars)`

指定した長さの文字列を生成する。各文字は `chars` から独立に選ばれる。

```cpp
string s = ranstring(10);          // 長さ10の小文字英字列
string binary = ranstring(20, "01");
string dna = ranstring(8, "ACGT");
```

`len` は0以上にし、`chars` には空でない文字列を渡す。

## 入力例

### 1個の整数

```cpp
string generate_input() {
    int n = ranint(1LL, 100LL);
    return to_string(n) + "\n";
}
```

### 長さ `N` の配列

```cpp
string generate_input() {
    int n = ranint(1LL, 20LL);

    ostringstream input;
    input << n << '\n';
    for (int i = 0; i < n; ++i) {
        input << ranint(1LL, 100LL);
        input << (i + 1 == n ? '\n' : ' ');
    }
    return input.str();
}
```

### 長さ `N` の文字列

```cpp
string generate_input() {
    int n = ranint(1LL, 30LL);
    string s = ranstring(n, "abc");

    ostringstream input;
    input << n << '\n';
    input << s << '\n';
    return input.str();
}
```

### 単純無向グラフ

自己ループと多重辺を避け、連結なグラフを生成する例。

```cpp
string generate_input() {
    int n = ranint(2LL, 10LL);

    vector<pair<int, int>> edges;
    set<pair<int, int>> used;

    // まず木を作って連結にする
    for (int v = 2; v <= n; ++v) {
        int u = ranint(1LL, v - 1);
        edges.push_back({u, v});
        used.insert({min(u, v), max(u, v)});
    }

    int max_m = n * (n - 1) / 2;
    int m = ranint(n - 1, min(max_m, n + 5));

    while ((int)edges.size() < m) {
        int u = ranint(1LL, n);
        int v = ranint(1LL, n);
        if (u == v) continue;

        pair<int, int> edge = {min(u, v), max(u, v)};
        if (used.contains(edge)) continue;

        used.insert(edge);
        edges.push_back({u, v});
    }

    shuffle(edges.begin(), edges.end(), rng);

    ostringstream input;
    input << n << ' ' << m << '\n';
    for (auto [u, v] : edges) {
        input << u << ' ' << v << '\n';
    }
    return input.str();
}
```

### 複数テストケース

```cpp
string generate_input() {
    int t = ranint(1LL, 5LL);

    ostringstream input;
    input << t << '\n';
    while (t--) {
        int n = ranint(1LL, 10LL);
        input << n << '\n';
        for (int i = 0; i < n; ++i) {
            input << ranint(-20LL, 20LL);
            input << (i + 1 == n ? '\n' : ' ');
        }
    }
    return input.str();
}
```

## 生成時のポイント

- 最小値・最大値・重複値など、壊れやすい境界ケースが出る範囲にする。
- `solver()` と `tester()` が前提とする制約を必ず満たす入力を作る。
- 順列なら `iota` と `shuffle`、重複なし集合なら `set` などを使って条件を保証する。
- グラフの連結性、自己ループ、多重辺、木かどうかなど、問題固有の条件を明示的に保証する。
- 愚直解の計算量に合わせて `N` を小さくする。テスト回数は `main()` の `TEST_COUNT` で調整する。
- 不一致時には `seed` と入力が標準エラーへ表示される。原因調査では、まず表示された入力を通常の標準入力として再実行する。

## 比較方法

出力は空白区切りのトークン列に変換して比較される。そのため、末尾の空白や改行数の違いは不一致にならない。
一方、トークンの個数・順番・内容の違いは不一致になる。浮動小数点数も文字列として厳密比較されるため、誤差を許容したい場合は比較処理を変更する必要がある。
