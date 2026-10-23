#pragma once

#include <algorithm>
#include <functional>
#include <limits>
#include <queue>
#include <utility>
#include <vector>

using ll = long long;

// 使用不同于 Kruskal 的类型名，两个头文件可以一起包含
struct WeightedEdge {
    int to;
    ll w;
};

const ll DIJKSTRA_INF = std::numeric_limits<ll>::max() / 4;

struct DijkstraResult {
    std::vector<ll> dist;
    std::vector<int> pre;
};

// 1-based 图；s 合法；边权非负，边权和有限最短距离均须小于 INF
inline DijkstraResult dijkstra(const std::vector<std::vector<WeightedEdge>>& g, int s) {
    std::vector<ll> dist(g.size(), DIJKSTRA_INF);
    std::vector<int> pre(g.size(), -1);
    using State = std::pair<ll, int>;
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    dist[s] = 0;
    pq.push({0, s});

    while (!pq.empty()) {
        auto cur = pq.top();
        pq.pop();
        ll d = cur.first;
        int u = cur.second;
        if (d != dist[u]) continue; // 丢弃过期距离快照

        for (const auto& e : g[u]) {
            ll nd = d + e.w;
            if (nd >= dist[e.to]) continue;
            dist[e.to] = nd;
            pre[e.to] = u;
            pq.push({nd, e.to});
        }
    }

    return {std::move(dist), std::move(pre)};
}

// pre 来自同一起点 s 的 dijkstra；返回点序列，不可达返回空
inline std::vector<int> restore_path(int s, int t, const std::vector<int>& pre) {
    std::vector<int> path;
    for (int v = t; v != -1; v = pre[v]) {
        path.push_back(v);
        if (v == s) {
            std::reverse(path.begin(), path.end());
            return path;
        }
    }
    return {};
}
