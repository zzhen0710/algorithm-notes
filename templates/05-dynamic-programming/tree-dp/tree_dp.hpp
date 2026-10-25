#pragma once

#include <algorithm>
#include <array>
#include <vector>
#include "../../06-graph-and-tree/dfs-order/dfs_order.hpp"

using ll = long long;

// 1-based 非空连通无向树；w 长度 n+1，所有中间和可用 ll 表示。
// 最大非空连通点集之和；dp[u] 必须包含 u，可同时连接多个孩子。
inline ll max_connected_sum(const std::vector<std::vector<int>>& g,
                            const std::vector<ll>& w, int root = 1) {
    int n = static_cast<int>(g.size()) - 1;
    auto t = tree_order(g, root);
    std::vector<ll> dp(n + 1);
    ll ans = w[root];
    for (int i = n; i; --i) {
        int u = t.order[i];
        dp[u] = w[u];
        for (int v : g[u])
            if (v != t.parent[u]) dp[u] += std::max(0LL, dp[v]);
        ans = std::max(ans, dp[u]);
    }
    return ans;
}

// 最大权独立集：相邻点不能同时选，允许空集，因此全负权返回 0。
inline ll max_tree_independent_set(const std::vector<std::vector<int>>& g,
                                  const std::vector<ll>& w, int root = 1) {
    int n = static_cast<int>(g.size()) - 1;
    auto t = tree_order(g, root);
    std::vector<std::array<ll, 2>> dp(n + 1);
    for (int i = n; i; --i) {
        int u = t.order[i];
        dp[u] = {{0, w[u]}};
        for (int v : g[u]) {
            if (v == t.parent[u]) continue;
            dp[u][0] += std::max(dp[v][0], dp[v][1]);
            dp[u][1] += dp[v][0];
        }
    }
    return std::max(dp[root][0], dp[root][1]);
}
