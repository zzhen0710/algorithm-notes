#pragma once

#include <algorithm>
#include <vector>

#include "../dsu/dsu.hpp"

using ll = long long;

// Adapted from ei1333/library (Unlicense), revision 088b4c8b49f85194099305d998e7113dc3d5dd2e:
// https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/graph/mst/kruskal.hpp
struct Edge {
    int u;
    int v;
    ll w;
};

struct MSTResult {
    ll weight; // 是否完成目标需另看 used，不能只看权重
    int used;
};

// 1-based 无向边表；1<=k<=n；原地排序，累加过程要求 ll 可表示
// k=1 求 MST；一般 k 求覆盖全部点、恰好 k 块的最小生成森林
inline MSTResult kruskal(int n, std::vector<Edge>& edges, int k = 1) {
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.w < b.w;
    });

    DSU d(n);
    MSTResult res{0, 0};

    for (const auto& e : edges) {
        if (d.components == k) break; // k=n 时不需要选边

        if (!d.unite(e.u, e.v)) continue;
        res.weight += e.w;
        ++res.used;
    }

    return res; // 成功条件：used == n-k；否则 weight 只是已选部分代价
}
