#pragma once
#include <algorithm>
#include <stack>
#include <utility>
#include <vector>

struct UndirectedGraph {
    struct Arc { int v, id; };
    std::vector<std::vector<Arc>> g;
    std::vector<std::pair<int, int>> edges;

    explicit UndirectedGraph(int n) : g(n + 1) {}

    int add_edge(int u, int v) {
        int id = (int)edges.size();
        edges.push_back({u, v});
        g[u].push_back({v, id});
        g[v].push_back({u, id}); // 同一无向边，两条邻接记录共用 id
        return id;
    }
};

struct CutResult {
    std::vector<bool> cut, bridge; // cut[u] 按点；bridge[id] 按原无向边
};

inline CutResult find_cuts(const UndirectedGraph& graph) {
    const auto& g = graph.g;
    int n = (int)g.size() - 1, timer = 0;
    std::vector<int> dfn(n + 1), low(n + 1), parent(n + 1);
    std::vector<int> pe(n + 1, -1), next(n + 1), child(n + 1);
    std::vector<bool> cut(n + 1, false), bridge(graph.edges.size(), false);
    std::stack<int> st;

    auto enter = [&](int u) {
        dfn[u] = low[u] = ++timer;
        st.push(u);
    };

    for (int s = 1; s <= n; ++s) {
        if (dfn[s]) continue;
        enter(s);

        while (!st.empty()) {
            int u = st.top();
            if (next[u] < (int)g[u].size()) {
                auto e = g[u][next[u]++];
                if (e.id == pe[u]) continue; // 只跳过来时那条边
                int v = e.v;
                if (!dfn[v]) {
                    parent[v] = u;
                    pe[v] = e.id;
                    ++child[u]; // 只统计 DFS 树孩子，不是度数
                    enter(v);
                } else {
                    low[u] = std::min(low[u], dfn[v]);
                }
                continue;
            }

            st.pop(); // u 的出边处理完，向父亲返回
            int p = parent[u];
            if (!p) {
                cut[u] = child[u] > 1; // 每棵 DFS 树的根都单独判断
            } else {
                low[p] = std::min(low[p], low[u]);
                if (low[u] > dfn[p]) bridge[pe[u]] = true;
                if (parent[p] && low[u] >= dfn[p]) cut[p] = true;
            }
        }
    }
    return {cut, bridge};
}
