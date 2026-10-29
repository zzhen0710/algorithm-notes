#pragma once
#include <algorithm>
#include <stack>
#include <utility>
#include <vector>

// 点 1..n，每条输入边只写一次；directed=true 表示有向图。
// ordered=true 求字典序最小点序列；失败返回 {}，无边返回 {1}。
inline std::vector<int> euler_trail(
    int n, const std::vector<std::pair<int, int>>& edges,
    bool directed = false, bool ordered = false) {
    int m = (int)edges.size();
    using Arc = std::pair<int, int>; // {邻点, 原边 id}
    std::vector<std::vector<Arc>> g(n + 1);
    std::vector<int> in(n + 1), out(n + 1);

    for (int id = 0; id < m; ++id) {
        int u = edges[id].first, v = edges[id].second;
        g[u].push_back({v, id});
        ++out[u]; ++in[v];
        if (!directed) {
            g[v].push_back({u, id});
            ++out[v]; ++in[u]; // 无向图中 out[u] 就是度数，自环贡献 2
        }
    }

    int s = 0;
    if (directed) {
        int plus = 0, minus = 0;
        for (int u = 1; u <= n; ++u) {
            int d = out[u] - in[u];
            if (d == 1) { ++plus; s = u; }
            else if (d == -1) ++minus;
            else if (d != 0) return {};
        }
        if (plus != minus || plus > 1) return {};
    } else {
        std::vector<int> odd;
        for (int u = 1; u <= n; ++u)
            if (out[u] & 1) odd.push_back(u);
        if (!odd.empty() && odd.size() != 2) return {};
        if (!odd.empty()) s = odd[0];
    }
    if (!s) {
        for (int u = 1; u <= n; ++u)
            if (out[u]) { s = u; break; }
        if (!s) s = 1;
    }

    if (ordered)
        for (auto& adj : g) std::sort(adj.begin(), adj.end());

    std::vector<int> next(n + 1), path;
    std::vector<bool> used(m);
    std::stack<int> st;
    st.push(s);
    while (!st.empty()) {
        int u = st.top();
        int& i = next[u];
        while (i < (int)g[u].size() && used[g[u][i].second]) ++i;
        if (i == (int)g[u].size()) {
            path.push_back(u); // 走不动才收尾，不在首次到达时写答案
            st.pop();
        } else {
            auto e = g[u][i++];
            used[e.second] = true; // 两个方向共享 id；永久标记这条原边
            st.push(e.first);
        }
    }
    if ((int)path.size() != m + 1) return {}; // 有边未覆盖，仍然失败
    std::reverse(path.begin(), path.end());
    return path;
}
