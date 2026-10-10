# Cycle-related topics to learn

AtCoderの実問題を調査した結果、価値はあるが今回の`cycle_base`へは追加しなかったものをまとめる。
既存ライブラリや解説コードをそのままコピーせず、必要になった時点で問題設定から再設計する。

## 1. Vertex-BCC / block-cut forest

### 学ぶ理由

「2頂点が同じ単純サイクルに含まれるか」は2-edge connectivityではなく、
cyclicな同一vertex-biconnected componentに属するかという問題。

block-cut forestまで構築すると、サイクルを含む一般グラフ上の問題を木DPへ落とせる。
cactusも「各blockが単辺または単純サイクル」として判定できる。

### 参考問題

- AGC071 C: https://atcoder.jp/contests/agc071/editorial/12588?lang=en
- ARC153 F: https://atcoder.jp/contests/arc153/editorial/5527?lang=en
- ABC318 G別解: https://atcoder.jp/contests/abc318/editorial/7723?lang=ja

### 実装前に決めること

- edge stackを用いるTarjanの非再帰実装
- articulation vertexが複数blockに属するAPI
- block-cut forestの頂点番号体系
- 多重辺、自己ループの所属規則
- `same_cyclic_block(u,v)` と単なる同一blockの区別

## 2. Static bridge forest and unique simple path

追加専用の橋は`incremental_bridge_graph`にあるが、静的snapshotとして
橋一覧、2-edge-connected component、bridge forestをまとめて返す機能は未実装。

一般グラフで`u-v`単純路が一意であることは、その経路の全辺が橋であることと同値。

参考:

- ARC039 D解説: https://img.atcoder.jp/arc039/editorial.pdf
- ABC266 F: https://atcoder.jp/contests/abc266/editorial/4709?lang=ja
- ABC288 C: https://atcoder.jp/contests/abc288/tasks/abc288_c?lang=en

LowLinkをコピーするのではなく、BCCと共通のhalf-edge/edge-stack基盤として設計したい。

## 3. Minimum weight cycle basis

現在取得できるのは、選んだspanning forestに対するfundamental basisの重みだけ。
これはminimum cycle basisではない。

非負重みではHorton候補を生成し、重み順のmatroid greedyで独立なcycleを選ぶ方法を学ぶ。
独立性判定は`M`-bit辺ベクトルではなく、現在のfundamental basisに対する
`rank`-bit chord座標へ圧縮できる。

学習項目:

- Horton candidate setとtie-breaking
- GF(2) weighted matroid greedy
- de Pina / Kavitha系アルゴリズム
- self-loop、多重辺、0重み辺
- 基底そのものの出力下界

`shortest_cycle`、`minimum fundamental basis`、`minimum cycle basis`は別問題なので混同しない。

参考となる最短サイクル問題:

- ABC022 C: https://atcoder.jp/contests/abc022/tasks/abc022_c
- ABC191 E: https://atcoder.jp/contests/abc191/tasks/abc191_e

## 4. MST replacement on fundamental cycles

MSTの非木辺を追加したときのfundamental cycle上へpath-chminを行う問題群。
cycleを列挙するより、tree path update/queryとしてHLDやDSU paintingを使うのが本質。

参考:

- AWC Jamming and Communication Network:
  https://atcoder.jp/contests/awc0074/editorial/19898?lang=en

学習対象:

- HLD range-chmin / point-query
- 重み順DSU painting
- replacement edge sensitivity
- MSTのcycle property / cut propertyの双対性

汎用HLDを既存ライブラリからコピーせず、必要なモノイド作用を決めて別モジュール化する。

## 5. Planar duality

平面グラフではprimalのcut spaceとdualのcycle spaceが対応する。
grid問題ならmin-cutをdual shortest pathへ落とせる場合がある。

参考:

- ABC413 G Big Banned Grid: https://atcoder.jp/contests/abc413/editorial/13403
- STPC2025_2 I: https://atcoder.jp/contests/stpc2025_2/editorial/14500

学習項目:

- rotation systemとhalf-edge
- face walkによる面列挙
- 外部面の扱い
- primal bridgeとdual self-loop
- Euler標数
- grid専用dualと一般平面埋め込みのAPI分離

## 6. Offline deletion and rollback

削除だけならクエリを逆順にして追加問題へ変換できる。

- ABC120 D: https://atcoder.jp/contests/abc120/tasks/abc120_d
- ABC264 E: https://atcoder.jp/contests/abc264/tasks/abc264_e

add/delete混在では、辺の有効区間を時間segment treeへ載せ、rollback DSUを使う。

学習・設計対象:

- rollback connectivity DSU
- rollback parity/potential DSU
- 各時刻の`rank = active_edges - n + components`
- segment tree over time
- witness基底をrollbackする場合のメモリ下界

現在のsmall-to-large rerootをそのままrollbackさせる設計は避ける。

## 7. Functional graph decomposition

functional graphのcycleは無向cycle spaceとは別物。
各弱連結成分が「有向cycle 1個＋流入する逆木」になる構造を専用クラスで扱う。

候補API:

```cpp
cycle_id[v]
cycle_position[v]
cycle_size[id]
distance_to_cycle[v]
cycle_entry[v]
kth_successor(v, k)
```

参考:

- https://atcoder.jp/contests/adt_hard_20240828_2/editorial/10192?lang=en
- https://atcoder.jp/contests/awc0150/editorial/24517?lang=en

indegree peel、reverse BFS、doublingを理解して独立モジュールとして書く。

## 8. General cochain / potential consistency

F2だけでなく、任意のAbelian group値を辺に持たせる。
全cycle評価が0であることは、辺ラベルが頂点potentialのcoboundaryであることと同値。

参考:

- DDCC2017 Final C: https://atcoder.jp/contests/ddcc2017-final/tasks/ddcc2017_final_c
- 解説: https://img.atcoder.jp/ddcc2017-final/editorial.pdf

候補クラス:

```cpp
potential_consistency<Group>
```

返したいもの:

- consistentか
- 頂点potential
- 矛盾を証明するfundamental cycle
- 1辺だけ変更して整合可能か

weighted/potential DSUの単なるコピーではなく、cycle certificateを第一級の戻り値にする。

## 9. Directed XOR walks

現在の`xor_cycle_basis`は無向辺を往復できることを使っている。
有向グラフでは同じcycle-spaceの商空間にはならない。

- ABC410 D: https://atcoder.jp/contests/abc410/tasks/abc410_d
- 解説: https://atcoder.jp/contests/abc410/editorial/13208?lang=en

小さいXOR幅なら `(vertex, xor_state)` の積グラフ探索が基本。
これは既存定石なのでcycle libraryへ追加せず、状態拡張の判断基準だけ学ぶ。

## 10. Euler decomposition / edge-disjoint circuits

辺集合がcycle spaceに属することは「全次数偶数」を保証するが、
指定個数の単純cycleやedge-disjoint circuitへ分けられる条件までは線形でない。

- AGC032 C Three Circuits:
  https://atcoder.jp/contests/agc032/tasks/agc032_c?lang=ja
  https://img.atcoder.jp/agc032/editorial.pdf

学習対象:

- Hierholzer法
- Euler tourのcycle splitting
- edge-disjoint cycle packing
- 線形なcycle-space条件と非線形な単純性・素性条件の境界

Hierholzer実装のコピーは今回追加しない。

## 11. Small-rank affine enumeration

ABC419 Gでは、固定`s-t` pathとcycle spaceのアフィン空間を列挙し、
simple pathだけを非線形filterする。

- 問題: https://atcoder.jp/contests/abc419/tasks/abc419_g
- 公式解説: https://atcoder.jp/contests/abc419/editorial/13662?lang=en

今回、境界付き合成、`cycle_core`、64辺bitmask、Gray-code座標列挙、
単純path判定までは`small_cycle_space64`として実装した。
残る学習項目:

- Gray code遷移に合わせた次数・連結性の差分更新
- rollback DSUによるmask連結性判定の高速化可能性
- meet-in-the-middleによるrank 30〜45程度への拡張
- path長分布をまとめて求める生成関数的手法

列挙自体は必ず指数時間なので、汎用APIには明示的な上限を要求したい。

## 12. Binary trie over canonical XOR quotient

ABC451 Gでは、cycle-imageで正規化した各頂点値をbinary trieへ入れることで
全頂点対のminimum XOR walkを処理する。

- 問題: https://atcoder.jp/contests/abc451/tasks/abc451_g?lang=en
- 解説: https://atcoder.jp/contests/abc451/editorial/18111

`xor_cycle_basis`にはcoset正規化まで追加した。binary trieは汎用データ構造なので、
cycle libraryへコピーせず別ライブラリとして学習・整備する。
