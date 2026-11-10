# Dijkstra：非负权最短路与路径还原

## 基础版：只求距离

日常比赛只查距离时直接用这版。沿用竞赛框架的 `ll`、`INF` 和 `Edge{to,w}`。图用 0-based 或 1-based 均可，起点和边端点须是合法下标。

```cpp
vector<ll> dijkstra(const vector<vector<Edge>>& g, int s) {
    vector<ll> dist(g.size(), INF);
    using State = pair<ll, int>; // {距离, 点}
    priority_queue<State, vector<State>, greater<State>> pq;

    dist[s] = 0;
    pq.push({0, s});

    while (!pq.empty()) {
        auto cur = pq.top();
        pq.pop();

        ll d = cur.first;
        int u = cur.second;
        if (d != dist[u]) continue; // 跳过旧距离

        for (const auto& e : g[u]) {
            ll nd = d + e.w;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
                pq.push({nd, e.to});
            }
        }
    }

    return dist;
}
```

边权非负，不可达点保留 `INF`。边权和有限最短距离都应小于 `INF`，可用 `LLONG_MAX / 4` 留出加法余量。

## 完整版：额外还原路径

要输出从 `s` 到 `t` 的点序列时，用 [dijkstra.hpp](dijkstra.hpp)。每次找到更短的走法，就记下最后一步从哪个点来。查询时沿 `pre` 从终点倒着走，再把点序列反过来。推导和例题见 [6.4 Dijkstra](../../../notes/06-graph-and-tree/04-dijkstra.md)。

- `WeightedEdge{to,w}`：一条邻接边，类型名与 Kruskal 的 `Edge` 区分，两个模板可一起包含。有向边存一次，无向边在调用处存两次。
- `dijkstra(g,s)`：图为 **1-based**，`g[0]` 留空，起点及全部端点合法；返回 `dist`、`pre`。**要求所有边权非负。**
- 不可达距离为 `DIJKSTRA_INF`，前驱为 `-1`；起点距离为 `0`。**每条边权和所有有限最短距离都必须小于该 INF**，不是任意 ll 大小的输入都满足本接口。
- `restore_path(s,t,res.pre)`：返回从 s 到 t 的点序列；不可达返回空，s=t 返回单点。前驱表必须来自同一起点的这次运行。
- 不修改图，所有状态每次调用重新创建；支持孤立点、零权、自环与重边，不保证字典序最小路径。

## 复杂度与取舍

设 n 为点数、m 为邻接记录数。时间上界 `O(n+m log(m+1))`，辅助空间 `O(n+m)`，路径还原 `O(n)`；无向边的正反记录都计入 m。

## 核验状态

完整版 C++11 独立编译与组合编译通过，未出现警告。与小图全源最短路参照对照，并核对还原路线、不可达、大权值、零权环、重复调用及输入不变。这里只记录本模板的本地核验，不增加平台 AC 或个人熟练记录。

[本章模板](../README.md) · [全部模板](../../README.md)
