#pragma once

#include <limits>
#include <vector>

using ll = long long;
const ll PRIM_INF = std::numeric_limits<ll>::max() / 4;

struct PrimResult {
    bool ok;
    ll weight;
    std::vector<int> parent;
};

// 1 基对称矩阵，n>=1，s 合法；无边=PRIM_INF，允许零/负权。
// 真实边 |w|<=1e12，n<=1e6；还需确保矩阵的时间/内存可接受。
// 成功时 parent[s]=0；失败为 {false,0,{}}。不修改 w。
inline PrimResult prim_dense(const std::vector<std::vector<ll>>& w, int s = 1) {
    int n = (int)w.size() - 1;
    std::vector<ll> best(n + 1, PRIM_INF);
    std::vector<int> parent(n + 1, -1);
    std::vector<bool> in(n + 1);
    best[s] = 0;
    parent[s] = 0;
    ll total = 0;

    for (int step = 0; step < n; ++step) {
        int u = -1;
        for (int v = 1; v <= n; ++v)
            if (!in[v] && (u == -1 || best[v] < best[u])) u = v;
        if (u == -1 || best[u] == PRIM_INF) return {false, 0, {}};

        in[u] = true;
        total += best[u];
        for (int v = 1; v <= n; ++v) {
            if (!in[v] && w[u][v] < best[v]) {
                best[v] = w[u][v];
                parent[v] = u;
            }
        }
    }
    return {true, total, parent};
}
