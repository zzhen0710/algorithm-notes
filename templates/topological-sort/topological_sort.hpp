#pragma once

#include <queue>
#include <vector>

// 1-based 有向图；不修改 g；返回能否得到完整拓扑序
// 有环时 order 只含可处理前缀，不能当作全部点的合法顺序
inline bool topo_sort(const std::vector<std::vector<int>>& g, std::vector<int>& order) {
    int n = static_cast<int>(g.size()) - 1;
    std::vector<int> indeg(n + 1);
    for (int u = 1; u <= n; ++u)
        for (int v : g[u]) ++indeg[v];

    std::queue<int> q;
    for (int u = 1; u <= n; ++u)
        if (indeg[u] == 0) q.push(u);

    order.clear();
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : g[u])
            if (--indeg[v] == 0) q.push(v);
    }

    return static_cast<int>(order.size()) == n;
}
