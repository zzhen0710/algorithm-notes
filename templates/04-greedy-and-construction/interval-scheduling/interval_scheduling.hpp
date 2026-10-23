#pragma once

#include <algorithm>
#include <vector>

using ll = long long;

struct Interval {
    ll l;
    ll r;
};

// 每段收益相同，只求最多条数；元素数可用 int 表示
// allow_touch=true: 非空 [l,r)，要求 l<r；false: 闭区间 [l,r]，允许 l==r
// 按值接收，原数组不变；每次调用重新初始化选择状态
inline int max_nonoverlap(std::vector<Interval> a, bool allow_touch = true) {
    std::sort(a.begin(), a.end(), [](const Interval& x, const Interval& y) {
        if (x.r != y.r) return x.r < y.r;
        return x.l < y.l; // 只用于固定同结束时间的处理顺序
    });

    bool noempty = false;
    ll last_r = 0;
    int ans = 0;

    for (const auto& cur : a) {
        if (noempty && (allow_touch ? cur.l < last_r : cur.l <= last_r)) {
            continue; // 未选入，不能改变已选边界
        }

        last_r = cur.r;
        noempty = true;
        ++ans;
    }

    return ans;
}
