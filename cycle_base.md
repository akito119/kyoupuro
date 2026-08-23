
# CycleSpace ライブラリ設計メモ

無向グラフのサイクルを \(\mathbb{F}_2\) 上の cycle space として扱う。

今回は計算量として最悪 \(O(NM)\) まで許容し、fundamental cycle basis を明示的に保持する。
その代わり、サイクルに関する問い合わせや構成をできるだけ豊富に提供する。

---

# 1. データ構造

## 1.1 グラフ

辺には必ず ID を振る。

```cpp
struct Edge {
    int to;
    int id;
};
```

```cpp
vector<vector<Edge>> g;
```

cycle space は本質的に頂点集合ではなく辺集合を \(\mathbb F_2\) 上で扱うため、
サイクルの内部表現も基本的には edge id を利用する。

---

## 1.2 Spanning Forest

fundamental cycle basis を作るため、各連結成分について spanning tree を構築する。

保持する情報:

```cpp
vector<int> parent;
vector<int> parent_edge;
vector<int> depth;
vector<int> component;
vector<bool> is_tree_edge;
```

意味:

```text
parent[v]
    spanning forest 上での親

parent_edge[v]
    parent[v] と v を結ぶ辺 ID

depth[v]
    spanning forest 上での深さ

component[v]
    元グラフ上の連結成分 ID

is_tree_edge[e]
    spanning forest に採用された辺か
```

---

## 1.3 Fundamental Cycle Basis

spanning forest に含まれない辺 \(e=(u,v)\) ごとに

\[
B_e
===

e
\oplus
\text{tree-path}(u,v)
\]

を作る。

これを fundamental cycle basis とする。

保持する:

```cpp
vector<vector<int>> basis_edges;
vector<vector<int>> basis_vertices;
```

ただしメモリを抑えたい場合は `basis_edges` のみを本体として、
`basis_vertices` は必要時に生成してもよい。

基底数は

\[
K=M-N+C
\]

である。

---

## 1.4 非木辺と基底の対応

fundamental cycle basis では、
各基底サイクルは固有の非木辺を1本持つ。

```cpp
vector<int> chord_of_basis;
vector<int> basis_of_chord;
```

これにより、

```text
非木辺 <-> 基底サイクル
```

の1対1対応を保持する。

---

## 1.5 辺・頂点から基底への逆引き

NMまで許容するため、所属関係を明示的に持つ。

```cpp
vector<vector<int>> basis_of_edge;
vector<vector<int>> basis_of_vertex;
```

意味:

```text
basis_of_edge[e]
    辺 e を含む基底サイクルの一覧

basis_of_vertex[v]
    頂点 v を含む基底サイクルの一覧
```

これらの総サイズは基底の総長

\[
L=\sum_i |B_i|
\]

に比例し、最悪 \(O(NM)\)。

---

## 1.6 基底サイクルのグループ

基底サイクル同士が辺を共有するなら同じグループとする。

```cpp
dsu basis_dsu;
```

辺 `e` を含む基底一覧

```cpp
basis_of_edge[e]
```

を見て、それらを全て Union する。

保持する:

```cpp
vector<int> cycle_component_of_basis;
vector<vector<int>> cycle_components_of_vertex;
```

重要:

頂点共有だけでは Union しない。

関節点で接する2サイクルは別グループとして扱う。

---

## 1.7 Cycle Space の座標

fundamental cycle basis を

\[
B_0,\dots,B_{K-1}
\]

とする。

任意の cycle-space element \(C\) は

\[
C=\bigoplus_i x_iB_i
\]

と一意に書ける。

fundamental basis では、
係数 \(x_i\) は対応する非木辺が \(C\) に含まれるかを見るだけで決まる。

そのため座標は

```cpp
vector<bool>
```

または

```cpp
dynamic_bitset
```

等で表す。

---

# 2. 実装するメソッド

初期API案:

```cpp
class CycleSpace {
public:
    explicit CycleSpace(int n);

    void add_edge(int u, int v);
    void build();

    int rank() const;
    int connected_components() const;

    const vector<vector<int>>& basis_edges() const;
    const vector<vector<int>>& basis_vertices() const;

    const vector<int>& bases_containing_edge(int e) const;
    const vector<int>& bases_containing_vertex(int v) const;

    bool in_some_cycle(int e) const;

    bool same_cycle(int u, int v) const;
    vector<int> get_cycle(int u, int v) const;

    vector<bool> coordinate(const vector<int>& edge_set) const;
    vector<int> compose(const vector<bool>& coef) const;

    int cycle_component_count() const;
    int cycle_component_of_basis(int i) const;
    const vector<int>& cycle_components_of_vertex(int v) const;
};
```

---

# 3. それぞれの方針

## 3.1 `build()`

### 目的

CycleSpace 全体の前計算を行う。

### 方針

1. DFS/BFSで spanning forest を構築
2. tree edge / non-tree edge を分類
3. 各 non-tree edge について fundamental cycle を復元
4. `basis_edges` を構築
5. `basis_of_edge` を構築
6. `basis_of_vertex` を構築
7. 共有辺を持つ基底を Union-Find で統合
8. 各頂点の cycle component 所属を構築

### 計算量

基底の総長を

\[
L=\sum_i|B_i|
\]

とすると、

\[
O(N+M+L\alpha(M))
\]

程度。

最悪 \(L=O(NM)\) なので、

\[
O(NM\alpha(M))
\]

程度を許容する。

---

## 3.2 `rank()`

### 目的

cycle space の次元を返す。

### 方針

\[
\operatorname{rank}=M-N+C
\]

または

```cpp
basis_edges.size()
```

を返す。

### 計算量

\[
O(1)
\]

---

## 3.3 `basis_edges()`

### 目的

fundamental cycle basis を辺ID列として取得する。

### 方針

`build()` で明示的に保存する。

各非木辺 \(e=(u,v)\) について、

```text
u から parent を辿る
v から parent を辿る
LCA で合流
非木辺 e を追加
```

してサイクルを復元する。

### 計算量

取得自体は参照を返せば

\[
O(1)
\]

構築時総計は

\[
O(L)
\]

最悪

\[
O(NM)
\]

---

## 3.4 `basis_vertices()`

### 目的

各基底サイクルを頂点列として取得する。

### 方針

`basis_edges` 構築時に同時に保存するか、
辺列から頂点列を復元する。

### 計算量

明示保存していれば取得は

\[
O(1)
\]

総メモリは

\[
O(L)
\]

---

## 3.5 `bases_containing_edge(e)`

### 目的

辺 `e` を含む基底サイクル一覧を返す。

### 方針

`build()` 中に各基底を走査し、

```cpp
basis_of_edge[e].push_back(basis_id);
```

する。

### 計算量

取得:

\[
O(1)
\]

列挙:

\[
O(\text{answer size})
\]

---

## 3.6 `bases_containing_vertex(v)`

### 目的

頂点 `v` を含む基底サイクル一覧を返す。

### 方針

各基底サイクルの頂点列を走査し、

```cpp
basis_of_vertex[v].push_back(basis_id);
```

する。

### 計算量

取得:

\[
O(1)
\]

列挙:

\[
O(\text{answer size})
\]

---

## 3.7 `in_some_cycle(e)`

### 目的

辺 `e` が少なくとも1つの単純サイクルに含まれるか判定する。

### 方針

fundamental cycle basis のどれかに `e` が含まれていれば true。

```cpp
return !basis_of_edge[e].empty();
```

これは

```text
e が bridge ではない
```

ことと同値。

### 計算量

\[
O(1)
\]

---

## 3.8 `same_cycle(u, v)`

### 目的

頂点 `u`,`v` を同時に含む単純サイクルが存在するか判定する。

### 方針

各頂点について

```cpp
cycle_components_of_vertex[v]
```

を保持する。

`u`,`v` の所属cycle component集合に共通要素があれば true。

```text
groups[u] ∩ groups[v] != empty
```

### 注意

関節点は複数のcycle componentに所属しうるため、

```cpp
component_of_vertex[v]
```

のような1値では管理できない。

### 計算量

所属groupをソート済みvectorで持つなら、

\[
O(|G_u|+|G_v|)
\]

程度。

必要なら各頂点のgroup集合を hash / bitset 等で高速化する。

---

## 3.9 `get_cycle(u, v)`

### 目的

`u`,`v` を両方含む単純サイクルを1本構成する。

最短性は要求しない。

### 方針

`same_cycle(u,v)==false` なら失敗。

true の場合、
共通cycle component内部で `u`,`v` を含む単純サイクルを構成する。

実装候補:

1. 該当component内の基底を利用して構成
2. componentを誘導部分グラフとして取り出し、
   2本の internally vertex-disjoint `u-v` path を構成
3. 必要なら BCC 的な構造を補助的に使う

### 計算量

初期実装では

\[
O(N+M)
\]

程度の追加探索を許容する。

---

## 3.10 `coordinate(edge_set)`

### 目的

cycle-space element を fundamental basis による座標へ変換する。

入力:

```cpp
vector<int> edge_set;
```

出力:

```cpp
vector<bool> coef;
```

### 方針

fundamental basis では、
各基底に固有の非木辺がある。

したがって、

```text
coef[i] = chord_of_basis[i] が edge_set に含まれるか
```

を見るだけでよい。

### 前提

入力 `edge_set` が本当に cycle space の元、
すなわち全頂点偶数次数の辺集合であること。

必要なら validation を行う。

### 計算量

辺集合をbool配列化すれば

\[
O(M)
\]

程度。

---

## 3.11 `compose(coef)`

### 目的

基底係数から cycle-space element を復元する。

\[
C=\bigoplus_i x_iB_i
\]

を計算する。

### 方針

辺ごとのbool配列を用意し、

```cpp
for (int i = 0; i < K; i++) {
    if (!coef[i]) continue;

    for (int e : basis_edges[i]) {
        used[e] ^= 1;
    }
}
```

する。

### 計算量

最悪

\[
O(L)
\]

したがって

\[
O(NM)
\]

以内。

bitsetで基底を持つ場合は word 単位 XOR に高速化可能。

---

## 3.12 `cycle_component_count()`

### 目的

共有辺で連結された基底グループの個数を返す。

### 方針

Union-Findのleader数を数える。

### 計算量

前計算後

\[
O(1)
\]

---

## 3.13 `cycle_component_of_basis(i)`

### 目的

基底 `i` が属するcycle componentを返す。

### 方針

`build()` 後に DSU leader を連番に圧縮して保存する。

### 計算量

\[
O(1)
\]

---

## 3.14 `cycle_components_of_vertex(v)`

### 目的

頂点 `v` が属するcycle component一覧を返す。

### 方針

`basis_of_vertex[v]` を見て、
それぞれの基底のcomponent IDへ写し、重複除去する。

### 計算量

前計算時:

\[
O(L\log L)
\]

程度まで許容。

取得時:

\[
O(1)
\]

---

# 4. 今回は別モジュールにするもの

以下は cycle space の線形構造そのものではなく、
最適化・単純性・最短路の問題なので別ライブラリに分ける。

```cpp
shortest_cycle();
shortest_cycle_through(u, v);
minimum_weight_cycle();
```

また、

```text
bridge
articulation point
BCC
Block-Cut Tree
```

は LowLink 系として独立させる。

---

# 5. 設計上の考え方

このライブラリの主目的は、

> 無向グラフの循環構造を、fundamental cycle basis を用いて明示的に保持し、
> cycle space 上の問い合わせ・構成を行えるようにする

こと。

特に今回は \(O(NM)\) まで許容するため、

```text
圧縮しすぎない
↓
基底を全部持つ
↓
所属関係も全部持つ
↓
問い合わせを簡単にする
```

方針を取る。

最初から LCA や遅延復元などで複雑化せず、
まずは正しさと使いやすさを優先する。

必要になった時点で、

```text
明示基底
→ bitset
→ LCA
→ 遅延復元
```

の順に高速化する。
