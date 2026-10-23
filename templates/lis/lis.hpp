#pragma once

#include <algorithm>
#include <vector>

using ll = long long;

// 0-based；len[i] 为必须以 a[i] 结尾的最长上升子序列长度
// strict=false 改求非下降；不还原实际子序列
inline std::vector<int> lis_ending(const std::vector<ll>& a, bool strict = true) {
    std::vector<ll> tail;
    std::vector<int> len;
    len.reserve(a.size());

    for (ll x : a) {
        auto it = strict ? std::lower_bound(tail.begin(), tail.end(), x)
                         : std::upper_bound(tail.begin(), tail.end(), x);
        len.push_back(static_cast<int>(it - tail.begin()) + 1);
        // push_back 可能使迭代器失效，所以先保存对应长度
        if (it == tail.end()) tail.push_back(x);
        else *it = x;
    }
    return len;
}

inline int lis_length(const std::vector<ll>& a, bool strict = true) {
    auto len = lis_ending(a, strict);
    return len.empty() ? 0 : *std::max_element(len.begin(), len.end());
}
