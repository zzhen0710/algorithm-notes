#pragma once

#include <array>
#include <cstdlib>
#include <functional>
#include <string>
#include <vector>

namespace digit_dp {
using ll = long long;

// x<=1e18，1<=m<=100；统计 [0,x] 中数位和模 m 为 0 的数，包含 0。
// x<0 返回 0；O(10*位数*m)，每次调用独立缓存。
inline ll count_digit_sum(ll x, int m) {
    if (x < 0) return 0;
    std::string s = std::to_string(x);
    int n = (int)s.size();
    std::vector<std::vector<ll>> memo(n, std::vector<ll>(m, -1));
    std::function<ll(int, int, bool)> dfs = [&](int pos, int rem, bool tight) -> ll {
        if (pos == n) return rem == 0;
        if (!tight && memo[pos][rem] != -1) return memo[pos][rem];
        ll ans = 0;
        int bound = s[pos] - '0', up = tight ? bound : 9;
        for (int d = 0; d <= up; ++d)
            ans += dfs(pos + 1, (rem + d) % m, tight && d == bound);
        if (!tight) memo[pos][rem] = ans;
        return ans;
    };
    return dfs(0, 0, true);
}

// 前提：0<=l<=r<=1e18；闭区间，不是 [l,r)。
inline ll count_digit_sum_range(ll l, ll r, int m) {
    return count_digit_sum(r, m) - count_digit_sum(l - 1, m);
}

// x<=1e18；统计 [0,x] 中非零位不超过 3 个的数，包含 0。
// 提前排除 cnt>3，故仅缓存 cnt=0..3；O(10*D*4)，空间 O(D*4+D)。
inline ll count_classy(ll x) {
    if (x < 0) return 0;
    std::string s = std::to_string(x);
    int n = (int)s.size();
    std::vector<std::array<ll, 4>> memo(n);
    for (auto& row : memo) row.fill(-1);
    std::function<ll(int, int, bool)> dfs = [&](int pos, int cnt, bool tight) -> ll {
        if (cnt > 3) return 0; // 在访问缓存前排除 cnt=4
        if (pos == n) return 1;
        if (!tight && memo[pos][cnt] != -1) return memo[pos][cnt];
        int bound = s[pos] - '0', up = tight ? bound : 9;
        ll ways = 0;
        for (int d = 0; d <= up; ++d)
            ways += dfs(pos + 1, cnt + (d != 0), tight && d == bound);
        if (!tight) memo[pos][cnt] = ways;
        return ways;
    };
    return dfs(0, 0, true);
}

// x<=1e18；统计 [1,x] 中相邻真实数位差至少为 2 的数，排除 0。
// last=10 表示尚未开始；O(10*D*11)，空间 O(D*11+D)。
inline ll count_windy(ll x) {
    if (x < 0) return 0;
    std::string s = std::to_string(x);
    int n = (int)s.size();
    std::vector<std::array<ll, 11>> memo(n);
    for (auto& row : memo) row.fill(-1);
    std::function<ll(int, int, bool)> dfs = [&](int pos, int last, bool tight) -> ll {
        if (pos == n) return last != 10;
        if (!tight && memo[pos][last] != -1) return memo[pos][last];
        int bound = s[pos] - '0', up = tight ? bound : 9;
        ll ways = 0;
        for (int d = 0; d <= up; ++d) {
            bool nt = tight && d == bound;
            if (last == 10 && d == 0) ways += dfs(pos + 1, 10, nt);
            else if (last == 10 || std::abs(last - d) >= 2)
                ways += dfs(pos + 1, d, nt);
        }
        if (!tight) memo[pos][last] = ways;
        return ways;
    };
    return dfs(0, 10, true);
}

} // namespace digit_dp
