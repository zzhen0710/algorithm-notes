#pragma once

#include <queue>
#include <vector>

// g 是 1-based 无向邻接表，g[0] 留空，外层长度可用 int 表示
// 编号合法，每条边存双向；允许孤立点、重边，自环必定失败
// 每次覆盖 color；成功得到完整方案，失败时不得输出部分染色作合法方案
inline bool bipartite_color(const std::vector<std::vector<int>>& g,
                            std::vector<int>& color) {
    int n = static_cast<int>(g.size()) - 1;
    color.assign(g.size(), -1);
    std::queue<int> q;

    for (int s = 1; s <= n; ++s) {
        if (color[s] != -1) continue;

        color[s] = 0; // 不同连通块可以独立翻色，起点任取 0
        q.push(s);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : g[u]) {
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false; // 已染色免于入队，仍要验证边约束
                }
            }
        }
    }

    return true;
}
