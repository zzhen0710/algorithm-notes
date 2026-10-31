#pragma once
#include <functional>
#include <unordered_set>
#include <vector>

// 每步代价1；完整状态须可复制、支持std::hash与==。
// next列出有限个合法后继，is_goal判定目标；两者只依赖完整状态。
// 0<=max_depth<INT_MAX，实际深度需适合调用栈；返回路径含起终点，空表示限内无解。
template<class State, class Goal, class Next>
std::vector<State> iddfs_path(const State& start, const Goal& is_goal,
                              const Next& next, int max_depth) {
    std::vector<State> path;
    std::unordered_set<State> in_path;
    std::function<bool(int)> dfs = [&](int left) -> bool {
        if (is_goal(path.back())) return true;
        if (left == 0) return false;
        for (const State& v : next(path.back())) {
            if (in_path.count(v)) continue;
            path.push_back(v);
            in_path.insert(v);
            if (dfs(left - 1)) return true;
            in_path.erase(v);
            path.pop_back();
        }
        return false;
    };
    for (int limit = 0; limit <= max_depth; ++limit) {
        path = {start};
        in_path.clear();
        in_path.insert(start);
        if (dfs(limit)) return path;
    }
    return {};
}
