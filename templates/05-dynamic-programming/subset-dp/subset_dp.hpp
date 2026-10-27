#pragma once

#include <algorithm>
#include <climits>
#include <vector>

namespace subset_dp {
using ll = long long;
constexpr ll INF = LLONG_MAX / 4;

// 0-based；n<=18，方阵；缺边填 INF，有限边 |w|<=1e12。
// 固定起点，每点恰好一次，无需回起点；无解 INF，空表 0。
inline ll shortest_visit_path(const std::vector<std::vector<ll>>& w, int start = 0) {
    int n = (int)w.size();
    if (!n) return 0;
    int limit = 1 << n;
    std::vector<std::vector<ll>> dp(limit, std::vector<ll>(n, INF));
    dp[1 << start][start] = 0;

    for (int mask = 0; mask < limit; ++mask) {
        for (int u = 0; u < n; ++u) {
            if (dp[mask][u] == INF) continue;
            for (int v = 0; v < n; ++v) {
                if ((mask >> v & 1) || w[u][v] == INF) continue;
                int next = mask | (1 << v);
                dp[next][v] = std::min(dp[next][v], dp[mask][u] + w[u][v]);
            }
        }
    }
    return *std::min_element(dp.back().begin(), dp.back().end());
}

// 0-based；n<=18，bonus 为 n*n；0<=m<=n；|base|、|bonus|<=1e12。
// 恰好选 m 个不同对象，收益=各 base+有向相邻 bonus；未设规则填 0。
inline ll max_select_value(const std::vector<ll>& base,
                           const std::vector<std::vector<ll>>& bonus, int m) {
    if (!m) return 0;
    int n = (int)base.size(), limit = 1 << n;
    std::vector<std::vector<ll>> dp(limit, std::vector<ll>(n, -INF));
    for (int u = 0; u < n; ++u) dp[1 << u][u] = base[u];

    ll ans = -INF;
    for (int mask = 1; mask < limit; ++mask) {
        int cnt = __builtin_popcount((unsigned)mask);
        if (cnt > m) continue;
        for (int u = 0; u < n; ++u) {
            if (dp[mask][u] == -INF) continue;
            if (cnt == m) {
                ans = std::max(ans, dp[mask][u]);
                continue;
            }
            for (int v = 0; v < n; ++v) {
                if (mask >> v & 1) continue;
                int next = mask | (1 << v);
                dp[next][v] = std::max(dp[next][v],
                    dp[mask][u] + base[v] + bonus[u][v]);
            }
        }
    }
    return ans;
}
} // namespace subset_dp
