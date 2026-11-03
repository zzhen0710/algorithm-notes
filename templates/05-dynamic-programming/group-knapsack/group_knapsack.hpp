#pragma once

#include <algorithm>
#include <vector>

using ll = long long;
const ll GROUP_NEG = -(1LL << 60);

struct GroupItem {
    int cost;
    ll value;
};

// groups[g] 是第 g 组的互斥候选；总占用不超过 cap。
// must_pick=false：每组至多选一；true：每组恰好选一，无解返回 GROUP_NEG。
inline ll group_knapsack(int cap, const std::vector<std::vector<GroupItem>>& groups,
                         bool must_pick = false) {
    std::vector<ll> dp(cap + 1, 0);
    for (const auto& group : groups) {
        for (int j = cap; j >= 0; --j) {
            ll best = must_pick ? GROUP_NEG : dp[j];
            for (const auto& x : group) {
                if (x.cost > j || dp[j - x.cost] == GROUP_NEG) continue;
                best = std::max(best, dp[j - x.cost] + x.value);
            }
            dp[j] = best; // 候选比较完再写入，cost=0 时也只读取一次旧 dp[j]。
        }
    }
    return dp[cap];
}
