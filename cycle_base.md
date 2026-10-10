# Cycle basis library

競技プログラミング向けの無向グラフ用ライブラリ。
全サイクルを明示保存せず、数学的なサイクル空間と構造的な橋・2-edge connectivityを分離する。

実装は `library/cycle_base.cpp` にあり、次の3クラスからなる。

```cpp
cycle_base                  // 暗黙fundamental cycle basis
cycle_core                  // terminalを保つ低cycle-rank kernel
small_cycle_space64         // 64辺以下のbit並列・指数列挙層
incremental_bridge_graph    // 追加専用bridge / 2-edge-connected components
xor_cycle_basis<UInt, Bits> // XORラベルへのcycle-space像
```

## 1. 数学的背景

グラフを1次元CW複体、係数体を `F_2` とみなす。

```text
C_1 = F_2^E
C_0 = F_2^V
boundary(uv) = u + v
Z_1 = ker(boundary)
```

2-cellを持たないため、`Z_1` はそのまま1次ホモロジーである。
辺集合が `Z_1` に属することと、全頂点の次数が偶数であることは同値。

連結成分数を `c` とすると、rank-nullityまたはEuler標数から

```text
dim Z_1 = m - n + c
```

となる。これは単純サイクルの個数ではなく、Eulerian辺集合の線形空間の次元である。
Eulerian辺集合の総数は `2^(m-n+c)`。

### Fundamental basis

spanning forest `T` を固定する。各非木辺（chord）`e=(u,v)` に対して

```text
B_e = e XOR path_T(u,v)
```

を作ると、全ての `B_e` が `Z_1` の基底になる。

各 `B_e` は固有のchord `e` を1本だけ持つため、基底行列のchord部分は単位行列になる。
したがってEulerian辺集合 `X` の基底座標は、単に `X` に含まれるchordの集合である。
Gaussian eliminationも全基底の展開も不要。

## 2. `cycle_base`

### 設計

辺追加時にUnion-Findで次を決める。

- 異なる連結成分を結ぶ辺: forest edge
- 同じ連結成分内の辺: chord、新しいbasis

forest edgeで小さい連結成分を大きい側へ接続し、小さい側だけ根を付け替える。
各頂点が小さい側に入る回数は高々 `O(log N)`。
根の付け替え時にbinary lifting表も更新する。

基底の正本は次だけである。

```cpp
struct basis_descriptor {
    int chord;
    int u;
    int v;
};
```

木上パスは問い合わせ時だけ復元する。

### 計算量

`Q` 回の追加全体:

```text
O(Q alpha(N) + N log^2 N)
```

メモリ:

```text
O(N log N + Q)
```

主な問い合わせ:

| 操作                               |                           計算量 |
| ---------------------------------- | -------------------------------: |
| `rank`, `connected_components` |                         `O(1)` |
| `connected`, `component_rank`  |                  `O(alpha(N))` |
| `basis_cycle_size/weight`        |                     `O(log N)` |
| `materialize_basis_*`            |            `O(log N + answer)` |
| `coordinate`                     | `O(N+input)`（妥当性検証込み） |
| `coordinate_unchecked`           |                     `O(input)` |
| `compose`                        |             `O(N+rank+answer)` |
| `sparse_coordinate`              |         `O(N+input log input)` |
| `compose_sparse`                 |          `O(N+k log k+answer)` |

`materialize_basis_*` の `answer` は出力するサイクル長。
長さ `K` のサイクルを返す処理には `Omega(K)` が必要なので、これは出力最適。

### 基本API

```cpp
cycle_base cy(n);
cy.reserve_edges(q);

int edge_id = cy.add_edge(u, v);          // weight=1
int edge_id = cy.add_edge(u, v, weight);

int r = cy.rank();
int c = cy.connected_components();
```

全情報は追加時に更新される。旧コードとの互換用に `build()` が存在するが、no-op。

基底IDと辺IDは追加後も変化しない。

```cpp
int b = cy.basis_of_chord(edge_id); // tree edgeなら-1
int e = cy.chord_of_basis(b);
auto descriptor = cy.basis(b);
```

自己ループは長さ1のbasis、多重辺の2本目は長さ2のbasisになり得る。

### 基底の復元

```cpp
vector<int> edges = cy.materialize_basis_edges(b);
vector<int> vertices = cy.materialize_basis_vertices(b);
```

辺列では最後の辺がchord。
全基底の一括展開は最悪 `Theta(NQ)` なので提供しない。

### Cycle-space membershipと座標

```cpp
bool ok = cy.is_cycle_space_element(edge_set);
vector<int> odd = cy.odd_degree_vertices(edge_set);

vector<bool> coef = cy.coordinate(edge_set);           // Eulerianをassert
vector<bool> coef = cy.coordinate_unchecked(edge_set); // chord射影のみ
vector<int> sparse = cy.sparse_coordinate(edge_set);
```

入力中の同じ辺IDは `F_2` としてXORされる。
`odd_degree_vertices` はmembership失敗時の証明書にもなる。
`coordinate` の妥当性検査は `assert` なので、`NDEBUG` 時の非Eulerian入力は利用者責任。

### 座標から辺集合を合成

```cpp
vector<int> edge_set = cy.compose(coef);
vector<int> edge_set = cy.compose_sparse(basis_ids);
```

選択chordの両端にparity差分を置き、forestをpostorderで1回走査する。
各basisを個別展開してXORしないため、基底の総長に依存しない。

`compose` の結果はEulerian部分グラフであり、単純サイクルとは限らない。

### その他

```cpp
int dim = cy.component_rank(v);
bool tree = cy.component_is_tree(v);
bool unicyclic = cy.component_is_unicyclic(v);
long long count = cy.cycle_space_size_mod(mod);
```

### 境界付きcycle space（affine fiber）

指定頂点集合 `T` だけを奇数次数にする辺集合を考える。これは境界方程式

```text
boundary(x) = T
```

の解である。各連結成分に含まれる `T` の頂点数が偶数なら解が存在し、
全解は特解 `x0` とcycle spaceのアフィン空間 `x0 + Z_1` になる。

```cpp
bool possible = cy.boundary_feasible(odd_vertices);

optional<vector<int>> edges =
    cy.compose_with_boundary(odd_vertices, cycle_coordinate);

optional<vector<int>> st_chain =
    cy.chain_from_coordinate(s, t, cycle_coordinate);

long long count = cy.boundary_fiber_size_mod(odd_vertices, mod);
```

解が存在する場合、その個数は常に `2^rank`。`compose_with_boundary` は
forest上のsubtree parityを1回伝播して `O(N+rank+answer)` で構成する。

これはARC115 Dの「指定奇次数集合に対し、木では解が一意、非木辺は自由」という構造と、
ABC419 Gの「固定`s-t` path + cycle space」を同じAPIへ抽象化したもの。

### 奇サイクル

```cpp
bool bipartite = cy.is_bipartite();
int basis_id = cy.odd_cycle_basis(); // 無ければ-1
vector<int> cycle = cy.odd_cycle_edges();
```

forestのdepth parityが同じ両端を結ぶchordは奇数長fundamental cycleを作る。
全basisが偶数ならcycle spaceの全要素も偶数なので、グラフは二部グラフ。

## 3. `cycle_core`

cyclomatic rankが小さい問題向けのkernelization。

```cpp
cycle_core core(n);
core.add_edge(u, v, weight);

auto compressed = core.compress({s, t});
```

次を線形時間で行う。

1. terminalでない次数0/1頂点を反復削除
2. terminalまたは次数が2でない頂点をjunctionにする
3. junction間の次数2 pathを重み付き1辺へ縮約

各kernel edgeは元の辺ID列を保持するため、列挙結果を元グラフへ復元できる。
terminal-freeな純cycle成分は、代表junctionを1つ選びself-loopへ圧縮する。

これはterminal間simple pathに使われない全ての辺を削除する最小coreではない。
terminal間の幹へ関節点1個だけで接続されたcyclic blockは残る。そこまで除去するには
vertex-BCCとblock-cut forest上のterminal Steiner subtreeが必要で、`to_learn.md`へ分離した。

連結グラフでterminalが2個、cycle rankを`K`とすると、不要部分の除去後は典型的に
kernel頂点数 `O(K)`、辺数 `O(K)` となる。ABC419 Gでは頂点数高々`2K+2`、
辺数高々`3K+1`となり、`2^K`列挙が現実的になる。

## 4. `small_cycle_space64`

`cycle_core`などで64辺以下へ圧縮できたグラフを、1つの`uint64_t`で表現する。

```cpp
auto compressed = core.compress({s, t});
small_cycle_space64 small(compressed);

uint64_t cycle = small.basis_mask(b);
uint64_t edge_set = small.compose_mask(coordinate);
uint64_t coordinate = small.coordinate_mask(edge_set);
bool eulerian = small.is_cycle_mask(edge_set);
auto chain = small.affine_mask({s, t}, coordinate);
```

`affine_mask(T, coordinate)`は、奇数次数頂点集合が`T`で、指定したcycle座標を持つ
辺集合を返す。存在しなければ`nullopt`。

### Gray-code列挙

```cpp
bool enumerated = small.enumerate_affine(
    {s, t},
    22, // 許容する最大rank
    [&](uint64_t edge_mask, uint64_t coordinate) {
        if (small.is_simple_path(edge_mask, s, t)) {
            long long length = small.mask_weight(edge_mask);
            // 集計
        }
    }
);
```

座標をGray code順に列挙するため、隣接状態の辺maskはfundamental cycle 1本分のXORだけ変わる。
列挙数は`2^rank`なので、事故防止の`max_rank`を必須引数にしている。

主な計算量:

| 操作                |                             計算量 |
| ------------------- | ---------------------------------: |
| `build`           | `O(N+M+rank*N)`、ただし`M<=64` |
| `compose_mask`    |        `O(popcount(coordinate))` |
| `coordinate_mask` |                        `O(rank)` |
| `is_cycle_mask`   |                         `O(N+M)` |
| `affine_mask`     |                      `O(N+rank)` |
| `is_simple_path`  |                         `O(N+M)` |
| `mask_weight`     |                           `O(M)` |
| 全列挙              |           `O(2^rank * callback)` |

`is_simple_path`は境界が`s,t`であるだけでなく、選択部分グラフが連結で全内部次数が2かも確認する。
したがって、離れたcycleを含む一般の`s-t` chainを単純pathから除外できる。

`cycle_core::result`から構築した場合、maskの各bitはkernel edgeに対応する。
元グラフの辺列へ戻すには`compressed.edges[e].original_edges`を展開する。

## 5. `incremental_bridge_graph`

サイクル空間の線形代数だけでは、tree edgeが現在bridgeかを高速には判定できない。
橋と2-edge connectivityは専用データ構造へ分離する。

2種類のUnion-Findと親森を用いる既知のincremental bridge algorithmで、
追加全体を償却 `O((N+Q) log N)` で処理する。

```cpp
incremental_bridge_graph graph(n);
int e = graph.add_edge(u, v);

int count = graph.bridge_count();
bool bridge = graph.is_bridge(e);
bool connected = graph.connected(u, v);
bool same = graph.same_2edge_component(u, v);
```

`same_2edge_component` は「同じ単純サイクル上にある」とは異なる。
後者はvertex-biconnected componentの問題であり、静的Tarjan/BCCを使う。

辺がbridgeでなくなった原因の追加辺も保持する。

```cpp
int chord = graph.cycle_witness_edge(e); // bridgeなら-1
int time = graph.became_cyclic_at(e);    // bridgeなら-1
```

`cycle_witness_edge(e)` が返すのはwitnessとなる追加辺IDであり、サイクルの辺列ではない。
必要なら追加履歴または別のforest path構造から復元する。

## 6. `xor_cycle_basis`

各辺に `UInt` 型のXORラベルがあるとする。辺集合上のXORは
cycle spaceから `F_2^Bits` への線形写像になる。

potential付きUnion-Findでforest pathのXORを保持し、chord `e=(u,v,w)` の追加時に

```text
potential[u] XOR potential[v] XOR w
```

を数値XOR基底へ挿入する。

```cpp
xor_cycle_basis<uint64_t> xb(n);
optional<uint64_t> cycle_value = xb.add_edge(u, v, label);

uint64_t maximum = xb.maximum_walk_xor(u, v);
uint64_t minimum = xb.minimum_walk_xor(u, v);
bool possible = xb.cycle_space_xor_representable(v, x);
int image_dimension = xb.image_rank(v);

uint64_t normalized = xb.normalize_cycle_coset(v, x);
uint64_t canonical = xb.canonical_vertex_value(v);
```

異なる連結成分のXOR基底は混ぜない。成分結合時に小さい側の数値基底を統合する。
`Bits=64` なら1回の基底操作は `O(64)`、成分結合は `O(64^2)` 以下の定数時間。
`Bits` を `UInt` の全bit数より小さくした場合、ラベルと問い合わせ値の上位bitが
すべて0であることを `assert` で検査する。

ここでのwalkは単純pathではない。基準となる `u-v` pathに任意のEulerian辺集合を加えた
1-chain、すなわち辺や頂点の再訪を許すwalkのXOR極値である。関節点にぶら下がったcycleも
利用できるため、単純pathの最大XORとは一致しない場合がある。

`normalize_cycle_coset(v,x)` は、成分のcycle-imageを加えて得られる値のうち最小のものを返す。
`canonical_vertex_value(v)` は、その連結成分の最小番号頂点を固定anchorとして、
anchorから`v`へのpotentialを同様に正規化する。
同一snapshot・同一成分では

```text
minimum_walk_xor(u,v)
= canonical_vertex_value(u) XOR canonical_vertex_value(v)
```

が成り立つ。成分併合でanchorが変わる場合や、辺追加でcycle-imageが広がる場合は
canonical値も変化するので、同一snapshot内でのみ利用する。

## 7. コアから外した機能

次は別問題なので、この高速コアには含めない。

- 頂点biconnected components / block-cut tree
- 2頂点を含む単純サイクルの構成
- 最短サイクル
- minimum weight cycle basis
- 全basisの一括展開
- 各辺・頂点を含む全basisの逆引き

非負重みのminimum cycle basisは、Horton候補を重み順にmatroid greedyする別モジュールが標準的。
候補cycleの独立性判定には辺数 `M` bitではなく、このライブラリのchord座標 `rank` bitを使える。

削除込みでrankだけ必要なら、辺の有効区間を時間segment treeへ載せ、rollback DSUで
各時刻の `m_active-n+components` を求める方が適切。

## 8. 設計上の下界

```text
全basisの総長:          Theta(NQ) になり得る
長さKのcycle witness:  Omega(K)
辺を含む全basis列挙:   Omega(rank) になり得る
compose/list出力:       Omega(answer)
```

したがって、高速な辺追加と全基底の常時明示保持は両立しない。
本ライブラリはdescriptorを正本にし、必要な出力だけを遅延生成する。
