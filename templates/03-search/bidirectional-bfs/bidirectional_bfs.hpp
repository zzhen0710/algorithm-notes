#pragma once
#include <unordered_map>
#include <vector>

namespace bidir_detail {
template<class State, class Step>
int layer(std::vector<State>& front,
          std::unordered_map<State, int>& dist,
          const std::unordered_map<State, int>& other, const Step& step) {
    std::vector<State> next_front;
    int best = -1;
    for (const State& u : front) {
        int nd = dist.at(u) + 1;
        for (const State& v : step(u)) {
            if (!dist.emplace(v, nd).second) continue;
            next_front.push_back(v);
            auto it = other.find(v);
            if (it != other.end()) {
                int length = nd + it->second;
                if (best == -1 || length < best) best = length;
            }
        }
    }
    front.swap(next_front); // 整层扩展，不能只处理一个状态就换边。
    return best;
}
}

// 全部边代价为1；next列后继，prev列原图前驱，两者必须描述同一张图。
// State须支持std::hash、==和复制；max_steps<0表示不限制，否则为总步数上限。
template<class State, class Next, class Prev>
int bidirectional_bfs(const State& start, const State& target,
                      const Next& next, const Prev& prev, int max_steps = -1) {
    if (start == target) return 0;
    std::vector<State> fs{start}, ft{target};
    std::unordered_map<State, int> ds{{start, 0}}, dt{{target, 0}};
    int a = 0, b = 0;
    while (!fs.empty() && !ft.empty()) {
        if (max_steps >= 0 && a + b >= max_steps) return -1;
        int answer;
        if (fs.size() <= ft.size()) {
            answer = bidir_detail::layer(fs, ds, dt, next);
            ++a;
        } else {
            answer = bidir_detail::layer(ft, dt, ds, prev);
            ++b;
        }
        if (answer != -1) return answer;
    }
    return -1;
}
