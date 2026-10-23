#pragma once

#include <algorithm>
#include <utility>
#include <vector>

using ll = long long;

// 半开区间 [l,r)，要求 l<=r；忽略空区间，相交或相接时合并
// 按值接收，不修改原输入；返回按左端点递增的非空合并区间
inline std::vector<std::pair<ll, ll>> merge_intervals(std::vector<std::pair<ll, ll>> a) {
    std::sort(a.begin(), a.end());
    std::vector<std::pair<ll, ll>> merged;
    for (const auto& seg : a) {
        if (seg.first == seg.second) continue;
        if (merged.empty() || seg.first > merged.back().second)
            merged.push_back(seg);
        else
            merged.back().second = std::max(merged.back().second, seg.second);
    }
    return merged;
}

// 除端点可用 ll 表示外，还要求每段长度及总长度均可用 ll 表示
inline ll union_length(std::vector<std::pair<ll, ll>> a) {
    ll ans = 0;
    for (const auto& seg : merge_intervals(std::move(a)))
        ans += seg.second - seg.first;
    return ans;
}
