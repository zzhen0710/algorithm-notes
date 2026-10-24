#pragma once

#include <algorithm>
#include <utility>
#include <vector>
#include "../dfs-order/dfs_order.hpp"

using ll = long long;

// 固定合法根、1-based 连通无向树；0 为不存在的祖先。
struct LCA {
    int n, lg;
    std::vector<int> depth;
    std::vector<std::vector<int>> up; // up[j][u]：上跳 2^j 步到达的节点。

    explicit LCA(const std::vector<std::vector<int>>& g, int root = 1)
        : n(static_cast<int>(g.size()) - 1), lg(1) {
        auto t = tree_order(g, root);
        depth = std::move(t.depth);
        while ((1LL << lg) <= n) ++lg;
        up.assign(lg, std::vector<int>(n + 1));
        up[0] = std::move(t.parent);
        for (int j = 1; j < lg; ++j)
            for (int u = 1; u <= n; ++u)
                up[j][u] = up[j - 1][up[j - 1][u]];
    }

    int jump(int u, int k) const {
        if (k < 0 || k > depth[u]) return 0;
        for (int j = lg - 1; j >= 0; --j)
            if ((k >> j) & 1) u = up[j][u];
        return u;
    }

    int lca(int u, int v) const {
        if (depth[u] < depth[v]) std::swap(u, v);
        u = jump(u, depth[u] - depth[v]);
        if (u == v) return u;
        for (int j = lg - 1; j >= 0; --j) {
            if (up[j][u] != up[j][v]) { // 不相遇才跳，始终留在 LCA 下方。
                u = up[j][u];
                v = up[j][v];
            }
        }
        return up[0][u];
    }

    ll distance(int u, int v) const { // 无权路径边数，不是点权／边权和。
        int w = lca(u, v);
        return 1LL * depth[u] + depth[v] - 2LL * depth[w];
    }
};
