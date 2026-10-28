# 問題演習の配置

演習解答は出典ごとのディレクトリ（`abc_practice`、`adt`、`weekday-contest`、`toketenai`、`paiza`）に置き、その中で主な解法によって分類する。

- グラフを頂点と辺として扱う問題は、必ずその出典の `graph/` 配下に置く。`graph/` 内では `bfs`、`dijkstra`、`scc`、`union_find`、`cycle_base`、`spanning_tree`、`traversal` など、中心となる手法で分ける。マス目の移動を BFS で探索する問題も `graph/bfs` とする。
- グラフ探索を使わないマス目の照射や移動シミュレーションは `simulation/` とする。
- 複数の手法を使う場合は、解法の中心となる手法を一つ選ぶ。例: 最短路と累積和を併用する問題は `graph/dijkstra/`。
- 日付別の演習は、出典が分かるように `YYYY-MM-DD-問題文字.cpp` というファイル名にする。未完成の解答や空の雛形も、推定できる手法で分類し、判定できないものは `unfinished/` に置く。

`library/` は再利用する実装、`AHC/` と `rand-test/` はそれぞれヒューリスティックコンテストとテスト用の作業場所なので、この演習分類には含めない。`paiza/` は `.gitignore` で除外されたローカル専用ディレクトリのまま扱う。
