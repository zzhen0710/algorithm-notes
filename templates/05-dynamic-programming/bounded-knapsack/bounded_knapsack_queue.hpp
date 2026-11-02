#pragma once

#include <algorithm>
#include <deque>
#include <utility>
#include <vector>
#include "bounded_knapsack.hpp" // 复用 BoundedItem，不重复定义。

// 同二进制版的容量上限语义；cost>0，count>=0，value>=0。
// 保存旧行，避免当前种类在新状态中被重复加入。
inline void add_bounded_queue(std::vector<ll>& dp, int cost, ll value, int count) {
    int cap = (int)dp.size() - 1;
    count = std::min(count, cap / cost);
    if (!count) return;
    std::vector<ll> old = dp;
    std::deque<std::pair<int, ll>> q;
    for (int r = 0; r < cost; ++r) {
        q.clear();
        for (int t = 0, pos = r; pos <= cap; ++t, pos += cost) {
            while (!q.empty() && q.front().first < t - count) q.pop_front();
            ll score = old[pos] - (ll)t * value;
            while (!q.empty() && q.back().second <= score) q.pop_back();
            q.push_back({t, score}); // s=t：取当前种类 0 件也合法。
            dp[pos] = q.front().second + (ll)t * value;
        }
    }
}

inline ll bounded_knapsack_queue(int cap, const std::vector<BoundedItem>& items) {
    std::vector<ll> dp(cap + 1);
    for (const auto& x : items) add_bounded_queue(dp, x.cost, x.value, x.count);
    return dp[cap];
}
