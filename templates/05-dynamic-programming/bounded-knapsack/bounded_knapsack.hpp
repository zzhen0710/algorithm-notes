#pragma once

#include <algorithm>
#include <vector>

using ll = long long;

struct BoundedItem {
    int cost;
    ll value;
    int count;
};

// dp[j]：已处理物品中，占用不超过 j 的最大价值；允许空方案。
// cost>0，count>=0；在现有 dp 上加一种物品，不清空数组。
inline void add_bounded(std::vector<ll>& dp, int cost, ll value, int count) {
    int cap = (int)dp.size() - 1;
    count = std::min(count, cap / cost);
    for (int block = 1; count > 0; block *= 2) {
        int take = std::min(block, count);
        count -= take;
        int w = take * cost;
        ll v = take * value;
        for (int j = cap; j >= w; --j)
            dp[j] = std::max(dp[j], dp[j - w] + v);
    }
}

// 每种物品最多 count 件；返回容量 cap 内最大价值，不修改 items。
inline ll bounded_knapsack(int cap, const std::vector<BoundedItem>& items) {
    std::vector<ll> dp(cap + 1);
    for (const auto& x : items) add_bounded(dp, x.cost, x.value, x.count);
    return dp[cap];
}
