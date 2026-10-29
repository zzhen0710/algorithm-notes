#pragma once
#include <algorithm>
#include <stack>
#include <vector>

struct SCCResult {
    int count;
    std::vector<int> id, size; // 点 1..n；分量 1..count，size[0] 不用
};

// 有向图，1-based；显式模拟 Tarjan DFS，不使用系统递归栈。
inline SCCResult tarjan_scc(const std::vector<std::vector<int>>& g) {
    int n = (int)g.size() - 1, timer = 0;
    std::vector<int> dfn(n + 1), low(n + 1), parent(n + 1), next(n + 1);
    std::vector<int> id(n + 1), size(1, 0);
    std::vector<bool> in(n + 1, false);
    std::stack<int> dfs, pending;

    auto enter = [&](int u) {
        dfn[u] = low[u] = ++timer;
        in[u] = true;
        dfs.push(u);
        pending.push(u);
    };

    for (int s = 1; s <= n; ++s) {
        if (dfn[s]) continue;
        enter(s);

        while (!dfs.empty()) {
            int u = dfs.top();
            if (next[u] < (int)g[u].size()) {
                int v = g[u][next[u]++];
                if (!dfn[v]) {
                    parent[v] = u;
                    enter(v);
                } else if (in[v]) {
                    low[u] = std::min(low[u], dfn[v]);
                }
                continue;
            }

            dfs.pop(); // u 的孩子全部处理完，模拟递归返回
            if (parent[u])
                low[parent[u]] = std::min(low[parent[u]], low[u]);

            if (low[u] == dfn[u]) {
                int c = (int)size.size();
                size.push_back(0);
                while (true) {
                    int v = pending.top();
                    pending.pop();
                    in[v] = false;
                    id[v] = c;
                    ++size[c];
                    if (v == u) break;
                }
            }
        }
    }
    return {(int)size.size() - 1, id, size};
}

// 保留跨分量重边；需要不同邻点数等语义时，调用者再去重。
inline std::vector<std::vector<int>> condensation(
    const std::vector<std::vector<int>>& g, const SCCResult& scc) {
    std::vector<std::vector<int>> dag(scc.count + 1);
    for (int u = 1; u < (int)g.size(); ++u)
        for (int v : g[u])
            if (scc.id[u] != scc.id[v])
                dag[scc.id[u]].push_back(scc.id[v]);
    return dag;
}
