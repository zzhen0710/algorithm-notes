#pragma once
#include <algorithm>
#include <functional>
#include <vector>

struct KnapsackItem {
    long long cost, value;
};

// 每件最多选一次，容量为上限，允许不选；cost>0，value>=0。
// cap、cost、全部 value 之和均不超过 1e18；适合物品数少的情况。
inline long long knapsack_bnb(const std::vector<KnapsackItem>& a, long long cap) {
    using ll = long long;
    int n = (int)a.size();
    std::vector<ll> suffix(n + 1);
    for (int i = n - 1; i >= 0; --i)
        suffix[i] = suffix[i + 1] + a[i].value;

    ll best = 0;
    std::function<void(int, ll, ll)> dfs = [&](int i, ll left, ll value) {
        best = std::max(best, value); // 停在这里、剩余全不选，也是合法方案。
        if (i == n || value + suffix[i] <= best) return;

        if (a[i].cost <= left) // 只剪掉“选当前物品”，仍需尝试不选。
            dfs(i + 1, left - a[i].cost, value + a[i].value);
        dfs(i + 1, left, value);
    };
    dfs(0, cap, 0);
    return best;
}
