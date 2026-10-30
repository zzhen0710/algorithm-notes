#pragma once
#include <algorithm>
#include <queue>
#include <vector>

struct Activity {
    int u, v;
    long long w;
};

struct CriticalPathResult {
    bool ok;
    long long duration;
    std::vector<long long> ve, vl, slack;
    std::vector<int> critical; // 零余量活动的原边 id
};

// 1..n 的 AOE 网；所有源事件可从 0 开始，全部活动都须完成。
// 多个终点共用项目截止时间 duration；有环或负耗时返回 ok=false，数组为空。
inline CriticalPathResult critical_path(int n, const std::vector<Activity>& edges) {
    CriticalPathResult r{};
    int m = (int)edges.size();
    std::vector<std::vector<int>> g(n + 1); // 保存出边 id
    std::vector<int> indeg(n + 1), order;
    for (int id = 0; id < m; ++id) {
        const auto& e = edges[id];
        if (e.w < 0) return {};
        g[e.u].push_back(id);
        ++indeg[e.v];
    }

    std::queue<int> q;
    for (int u = 1; u <= n; ++u)
        if (!indeg[u]) q.push(u);
    r.ve.assign(n + 1, 0);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int id : g[u]) {
            const auto& e = edges[id];
            r.ve[e.v] = std::max(r.ve[e.v], r.ve[u] + e.w);
            if (--indeg[e.v] == 0) q.push(e.v);
        }
    }
    if ((int)order.size() != n) return {}; // 不能使用有环图的部分状态
    r.duration = *std::max_element(r.ve.begin() + 1, r.ve.end());

    r.vl.assign(n + 1, r.duration);
    for (int i = n - 1; i >= 0; --i) {
        int u = order[i];
        for (int id : g[u]) {
            const auto& e = edges[id];
            r.vl[u] = std::min(r.vl[u], r.vl[e.v] - e.w);
        }
    }

    r.slack.resize(m);
    for (int id = 0; id < m; ++id) {
        const auto& e = edges[id];
        r.slack[id] = r.vl[e.v] - e.w - r.ve[e.u];
        if (r.slack[id] == 0) r.critical.push_back(id);
    }
    r.ok = true;
    return r;
}
