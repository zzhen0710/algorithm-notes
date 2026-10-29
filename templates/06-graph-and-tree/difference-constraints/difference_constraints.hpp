#pragma once

#include "../negative-shortest-path/negative_shortest_path.hpp"

struct DifferenceConstraints {
    using ll = long long;
    std::vector<std::vector<negative_shortest::AdjEdge>> g;

    explicit DifferenceConstraints(int n) : g(n + 1) {}

    // x[i]-x[j] <= c：右边的 x[j] 给 x[i] 提供上界。
    void add_le(int i, int j, ll c) { g[j].push_back({i, c}); }

    // x[i]-x[j] >= c，转成 x[j]-x[i] <= -c。
    void add_ge(int i, int j, ll c) { add_le(j, i, -c); }

    // x[i]-x[j] == c，必须同时限制两个方向。
    void add_eq(int i, int j, ll c) {
        add_le(i, j, c);
        add_le(j, i, -c);
    }

    // negative_cycle=true 表示无解；否则 dist[1..n] 是一组可行解。
    negative_shortest::Result solve() const {
        return negative_shortest::spfa(g, 0);
    }
};
