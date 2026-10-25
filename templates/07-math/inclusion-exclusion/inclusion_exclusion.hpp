#pragma once

#include <vector>

using ll = long long;

// inter 长度为 2^k，0<=k<=20；非空 mask 存真实交集大小。
// inter[0] 不使用；中间累加须能用 ll 表示。GCC/Clang 内建计数。
inline ll inclusion_exclusion(const std::vector<ll>& inter) {
    ll ans = 0;
    int limit = static_cast<int>(inter.size());
    for (int mask = 1; mask < limit; ++mask) {
        if (__builtin_popcount(static_cast<unsigned>(mask)) & 1)
            ans += inter[mask];
        else
            ans -= inter[mask];
    }
    return ans;
}
