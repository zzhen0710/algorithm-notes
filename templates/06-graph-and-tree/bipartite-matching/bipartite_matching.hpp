#pragma once

#include <algorithm>
#include <functional>
#include <utility>
#include <vector>

struct MatchingResult {
    int count;
    std::vector<int> match_r; // 右点配给哪个左点；0 表示空闲
};

// g 有 n+1 项，只存左到右的邻接；左右分别 1-based，右点在 1..m
inline MatchingResult max_matching(const std::vector<std::vector<int>>& g, int m) {
    int n = static_cast<int>(g.size()) - 1;
    std::vector<int> match_r(m + 1), vis(m + 1);

    std::function<bool(int)> dfs = [&](int u) -> bool {
        for (int v : g[u]) {
            if (vis[v]) continue;
            vis[v] = 1;
            if (match_r[v] == 0 || dfs(match_r[v])) {
                match_r[v] = u; // 先给旧搭档找到去处，再接管右点
                return true;
            }
        }
        return false;
    };

    int count = 0;
    for (int u = 1; u <= n; ++u) {
        std::fill(vis.begin(), vis.end(), 0); // 只清本轮标记，保留已有配对
        if (dfs(u)) ++count;
    }
    return {count, std::move(match_r)};
}
