# 有向图反向可达性

题目给了终点，问「哪些点能走到这里」，就从终点沿**原图入边**找前驱。读到原边 `u -> v` 时，同时写入 `g[u].push_back(v)` 和 `rg[v].push_back(u)`。例如原图 `A → B → T`，在反图从 `T` 出发会依次找到 `B` 和 `A`。

$$
u\leadsto t\ \text{于原图}
\iff
t\leadsto u\ \text{于反图}.
$$

原路径逐边反向后，首尾互换，边数与总权值保留。对任意一条反图路径再反向，也能还原成原图路径。

| 要回答的问题 | 从哪里出发 | 怎么遍历 |
| --- | --- | --- |
| 谁能到达一个终点或出口？ | 终点 | 反图 BFS 或 DFS |
| 谁能到达任一目标，或能走到一个已知环？ | 目标集合中的点，或环上的所有点 | 反图多源遍历，所有种子起初就标记 |
| 每个点到同一终点的最短距离是多少？ | 终点 | 单位边权用 BFS，一般非负边权用 Dijkstra。反边沿用原权值 |

建反图和一次可达性遍历合计 $O(n+m)$ 时间与空间。双向 BFS 的终点一侧也按这个方向找前驱。[双向 BFS 建反图示例](../../notes/03-search/05-bidirectional-bfs.md#具体建图与调用)

## P2296 寻找道路：用反向可达性筛点

本题要求路径上的每个点，其**原图出边的所有终点**都能在原图到达 `t`。通过反向 BFS 得到 `can`，再筛出允许进入答案路径的点 `ncan`，最后沿原图做一次 BFS。代码沿用竞赛框架的 `ll` 与 `INF`。[P2296 原题](https://www.luogu.com.cn/problem/P2296)

```cpp
void solve() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> g(n + 1), rg(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        rg[v].push_back(u);
    }

    int s, t;
    cin >> s >> t;

    // 1. 反向 BFS，标记在原图里能走到 t 的点。
    vector<bool> can(n + 1);
    queue<int> q;
    can[t] = true;
    q.push(t);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : rg[u]) {
            if (can[v]) continue;
            can[v] = true;
            q.push(v);
        }
    }

    // 2. 谁指向不可达点，谁就不能走进答案路径。
    vector<bool> ncan(can);
    for (int u = 1; u <= n; ++u)
        if (!can[u])
            for (int v : rg[u])
                ncan[v] = false;

    if (!ncan[s]) {
        cout << "-1\n";
        return;
    }

    // 3. 沿原图做 BFS，只经过允许的点。
    vector<ll> dist(n + 1, INF);
    dist[s] = 0;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g[u]) {
            if (ncan[v] && dist[v] == INF) {
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }

    cout << (dist[t] == INF ? -1 : dist[t]) << '\n';
}
```

- `can[u]` 回答「从 `u` 能不能到 `t`」。在 `rg` 中从 `t` 出发，第一次找到的每个前驱都打上标记。
- `ncan[u]` 回答「`u` 可达 `t`，且它的每个直接后继也可达 `t` 吗」。遍历每个 `can[u] == false` 的点，沿 `rg[u]` 把直接指向它的点标为不可选。
- 最后一轮沿 `g` 走，只访问 `ncan` 为真的点。它检查筛完以后是否还剩一条合法路径，也求出最短长度。三步合计 $O(n+m)$ 时间与空间。

筛点时一直读原始 `can`。例如下图中 `a` 能到 `t`，但它也指向走不到 `t` 的 `x`，所以 `a` 不能进入答案路径。`s` 的直接后继 `a`、`t` 都能在原图到 `t`，因此 `s` 仍可走直达边。把 `a` 的淘汰继续传给 `s`，就会误删这条答案。

```text
s ──→ a ──→ t
│     │     ↑
│     ↓     │
│     x     │
└───────────┘
```

[图的基本遍历](../../notes/06-graph-and-tree/01-graph-basics.md) · [本章范式](README.md)
