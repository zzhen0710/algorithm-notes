#pragma once

#include <algorithm>
#include <vector>

using ll = long long;

struct BiItem {
    int cost1, cost2; // 选择这一件时，同时消耗的两种资源
    ll value;
};

// 每件至多一次；两种资源的总占用分别不超过 cap1、cap2。
inline ll knapsack_2d(int cap1, int cap2, const std::vector<BiItem>& items) {
    std::vector<std::vector<ll>> dp(cap1 + 1, std::vector<ll>(cap2 + 1, 0));

    for (const auto& x : items) {
        // 任意一种资源都放不下，这件就不能选。
        if (x.cost1 > cap1 || x.cost2 > cap2) continue;

        for (int j = cap1; j >= x.cost1; --j) {
            for (int k = cap2; k >= x.cost2; --k) {
                dp[j][k] = std::max(dp[j][k],
                                    dp[j - x.cost1][k - x.cost2] + x.value);
            }
        }
    }

    return dp[cap1][cap2];
}
