#pragma once

#include <limits>
#include <queue>
#include <vector>

namespace negative_shortest {
using ll = long long;
constexpr ll INF = std::numeric_limits<ll>::max() / 4;

struct Edge { int u, v; ll w; };
struct AdjEdge { int v; ll w; };
struct Result {
    std::vector<ll> dist;
    bool negative_cycle;
};

// 1-based；1<=s<=n 检查从 s 可达的负环；s=0 等价于虚拟源连向全部点。
// n>=1，n+1、实际边数 m 可用 int 表示且内存足够；令 W 为边权绝对值上限。
// 要求数学意义上的 n*m*W<=1e18，保证检测期间的中间加法也安全。
// negative_cycle=true 时 dist 只是中间结果，不能当作最终最短路。
inline Result bellman_ford(int n, const std::vector<Edge>& edges, int s = 1) {
    std::vector<ll> d(n + 1, s == 0 ? 0 : INF);
    if (s != 0) d[s] = 0;
    for (int round = 1; round <= n; ++round) {
        bool changed = false;
        for (const auto& e : edges) {
            if (d[e.u] == INF || d[e.v] <= d[e.u] + e.w) continue;
            d[e.v] = d[e.u] + e.w;
            changed = true;
            if (round == n) return {d, true};
        }
        if (!changed) return {d, false};
    }
    return {d, false};
}

// 同一契约；g.size()==n+1。inq 只表示当前是否在队列中，不是永久访问标记。
// hops[v] 是当前这次距离更新所对应走法的边数，不是累计松弛／入队次数。
inline Result spfa(const std::vector<std::vector<AdjEdge>>& g, int s = 1) {
    int n = (int)g.size() - 1;
    std::vector<ll> d(n + 1, s == 0 ? 0 : INF);
    std::vector<int> hops(n + 1);
    std::vector<bool> inq(n + 1, false);
    std::queue<int> q;
    if (s == 0) {
        for (int u = 1; u <= n; ++u) q.push(u), inq[u] = true;
    } else {
        d[s] = 0;
        q.push(s);
        inq[s] = true;
    }
    while (!q.empty()) {
        int u = q.front(); q.pop();
        inq[u] = false;
        for (const auto& e : g[u]) {
            if (d[e.v] <= d[u] + e.w) continue;
            d[e.v] = d[u] + e.w;
            hops[e.v] = hops[u] + 1;
            if (hops[e.v] >= n) return {d, true};
            if (!inq[e.v]) q.push(e.v), inq[e.v] = true;
        }
    }
    return {d, false};
}
} // namespace negative_shortest
