#pragma once

#include <algorithm>

using ll = long long;

// n>=0；非法 m 返回 0；合法参数要求每步乘法中间值都能用 ll 表示
// 精确整数，不取模，不检测溢出；m=0 或 m=n 返回 1
inline ll C(ll n, ll m) {
    if (m < 0 || m > n) {
        return 0;
    }

    m = std::min(m, n - m);
    ll ans = 1;

    for (ll i = 1; i <= m; ++i) {
        ans = ans * (n - m + i) / i; // 每轮先乘后整除，不能随意调换顺序
    }

    return ans;
}
