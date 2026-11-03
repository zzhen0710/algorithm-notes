#pragma once
#include <vector>
#include "../../06-graph-and-tree/topological-sort/topological_sort.hpp"

// 1-based 图；n>=1，所有边端点均在 1..n，g[0] 不使用。
// 非终点的出边概率非负且总和为1；每次到达同一状态时后续规律相同。
// cost 为非负有限代价；无出边即结束。只读，无全局状态。
struct ProbEdge {
    int v;
    double cost, p;
};

// 1-based DAG；到没有出边的点就结束。
// 每个非终点的出边概率非负且总和为 1；概率由完整的当前状态决定。
// 返回从每个点出发的期望剩余代价；有环或没有有效点时返回空数组。
inline std::vector<double> dag_expected_cost(const std::vector<std::vector<ProbEdge>>& g) {
    int n = (int)g.size() - 1;
    if (n < 1) return {};

    // 只把终点编号交给已有的拓扑排序；概率与代价仍留在 g 中。
    std::vector<std::vector<int>> to(n + 1);
    for (int u = 1; u <= n; ++u)
        for (const auto& e : g[u])
            to[u].push_back(e.v);

    std::vector<int> order;
    if (!topo_sort(to, order)) return {};

    std::vector<double> ex(n + 1, 0.0);

    // 当前点依赖后继，因此反过来遍历拓扑序。
    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        for (const auto& e : g[u])
            ex[u] += e.p * (e.cost + ex[e.v]);
    }
    return ex;
}
