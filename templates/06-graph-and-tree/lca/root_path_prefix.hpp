#pragma once

#include <vector>
#include "../dfs-order/dfs_order.hpp"

using ll = long long;

// t 已完成固定根遍历，weight 按节点 1..n；所有中间和须能用 ll 表示。
// 返回 sum[u]：从根到 u、包含两端的节点权值和；sum[0]=0。
inline std::vector<ll> root_path_prefix(const DFSOrder& t,
                                        const std::vector<ll>& weight) {
    int n = static_cast<int>(weight.size()) - 1;
    std::vector<ll> sum(n + 1);
    for (int i = 1; i <= n; ++i) {
        int u = t.order[i];
        sum[u] = sum[t.parent[u]] + weight[u];
    }
    return sum;
}
