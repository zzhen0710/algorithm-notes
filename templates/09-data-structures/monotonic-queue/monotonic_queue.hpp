#pragma once

#include <deque>
#include <vector>

using ll = long long;

// 0-based；第 t 项返回窗口 [t,t+k-1] 的最大值下标，相等时取最右
// k<=0 或 k>n 返回空；不修改输入
inline std::vector<int> window_max_positions(const std::vector<ll>& a, int k) {
    int n = static_cast<int>(a.size());
    std::vector<int> ans;
    if (k <= 0 || k > n) return ans;
    ans.reserve(n - k + 1);
    std::deque<int> q;

    for (int i = 0; i < n; ++i) {
        int left = i - k + 1;
        while (!q.empty() && q.front() < left) q.pop_front();
        while (!q.empty() && a[q.back()] <= a[i]) q.pop_back();
        q.push_back(i);
        if (left >= 0) ans.push_back(q.front());
    }
    return ans;
}
