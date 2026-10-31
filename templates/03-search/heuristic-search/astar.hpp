#pragma once
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <vector>

// 每步代价1；estimate是非负整数下界，目标处为0。
// max_steps默认-1表示不设人为上限；g+h须小于INT_MAX。返回路径含起终点，空表示耗尽或限内无解。
template<class State, class Goal, class Next, class Heuristic>
std::vector<State> astar_path(const State& start, const Goal& is_goal,
                     const Next& next, const Heuristic& estimate, int max_steps = -1) {
    struct Item {
        int f, g;
        State u;
    };
    struct Cmp {
        bool operator()(const Item& a, const Item& b) const {
            if (a.f != b.f) return a.f > b.f; // 先取f小的。
            return a.g < b.g; // 同f时先取g大的，不承诺路径字典序。
        }
    };
    std::priority_queue<Item, std::vector<Item>, Cmp> pq;
    std::unordered_map<State, int> best_g;
    std::unordered_map<State, State> pre;

    int first = estimate(start);
    if (max_steps >= 0 && first > max_steps) return {};
    best_g[start] = 0;
    pq.push({first, 0, start});

    while (!pq.empty()) {
        auto cur = pq.top();
        pq.pop();
        if (cur.g != best_g.at(cur.u)) continue; // 丢弃旧距离快照。

        // 目标从堆中取出时结束，不是在第一次生成目标时结束。
        if (is_goal(cur.u)) {
            std::vector<State> path{cur.u};
            while (!(path.back() == start))
                path.push_back(pre.at(path.back()));
            std::reverse(path.begin(), path.end());
            return path;
        }
        if (max_steps >= 0 && cur.g == max_steps) continue;

        for (const State& v : next(cur.u)) {
            int ng = cur.g + 1;
            auto it = best_g.find(v);
            if (it != best_g.end() && it->second <= ng) continue;

            int f = ng + estimate(v);
            if (max_steps >= 0 && f > max_steps) continue; // 下界都超预算，不必入堆。
            best_g[v] = ng;
            auto p = pre.emplace(v, cur.u);
            if (!p.second) p.first->second = cur.u;
            pq.push({f, ng, v}); // 允许更短路线再次更新、展开同一状态。
        }
    }
    return {};
}
