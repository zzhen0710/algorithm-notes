#pragma once

#include <vector>

using ll = long long;

// 统计进入函数时 [l,r] 中的严格逆序对，结束后该段非降序
// tmp 与 a 等长；当前区间合法，空区间只由入口传入 [0,-1]
inline ll merge_count(std::vector<ll>& a, std::vector<ll>& tmp, int l, int r) {
    if (l >= r) return 0;

    int mid = l + (r - l) / 2;
    ll ans = merge_count(a, tmp, l, mid);
    ans += merge_count(a, tmp, mid + 1, r);

    int i = l, j = mid + 1;
    int p = l;

    while (i <= mid && j <= r) {
        if (a[i] <= a[j]) {
            // 相等不构成严格逆序对，先取左项
            tmp[p++] = a[i++];
        } else {
            // 右项小于左半剩余的所有项，一次计入整段贡献
            ans += mid - i + 1;
            tmp[p++] = a[j++];
        }
    }

    while (i <= mid) tmp[p++] = a[i++];
    while (j <= r) tmp[p++] = a[j++];

    for (int pos = l; pos <= r; ++pos) {
        a[pos] = tmp[pos];
    }

    return ans;
}

// 0 下标闭区间；元素数量不超过 INT_MAX；会将调用方数组排序
// 需要保留原顺序时，先复制再调用；每次调用单独初始化缓冲区
inline ll count_inversions(std::vector<ll>& a) {
    int n = static_cast<int>(a.size());
    std::vector<ll> tmp(n);

    return merge_count(a, tmp, 0, n - 1);
}
