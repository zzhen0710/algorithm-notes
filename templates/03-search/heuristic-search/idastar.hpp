#pragma once
#include <algorithm>
#include <climits>
#include <functional>
#include <unordered_set>
#include <vector>

// 每步代价1、非负可采纳h；max_steps默认-1，不设人为上限。
// 返回路径含起终点；无解需有限可达状态图，g+h须小于INT_MAX。
template<class State, class Goal, class Next, class Heuristic>
std::vector<State> idastar_path(const State& start, const Goal& is_goal,
                       const Next& next, const Heuristic& estimate, int max_steps = -1) {
    const int INF = INT_MAX;
    std::vector<State> path;
    std::unordered_set<State> in_path;
    int bound = estimate(start), next_bound;

    std::function<bool(int)> dfs = [&](int g) -> bool {
        int f = g + estimate(path.back());
        if (f > bound) {
            next_bound = std::min(next_bound, f); // 记录刚超出本轮门槛的最小f。
            return false;
        }
        if (is_goal(path.back())) return true;
        if (max_steps >= 0 && g == max_steps) return false;

        for (const State& v : next(path.back())) {
            if (in_path.count(v)) continue;
            path.push_back(v);
            in_path.insert(v);
            if (dfs(g + 1)) return true; // 成功时保留路径。
            in_path.erase(v);
            path.pop_back();
        }
        return false;
    };

    while (max_steps < 0 || bound <= max_steps) {
        path = {start};
        in_path.clear();
        in_path.insert(start);
        next_bound = INF;
        if (dfs(0)) return path;
        if (next_bound == INF) return {}; // 所有分支耗尽，没有更大门槛可试。
        bound = next_bound;
    }
    return {};
}
