#pragma once

#include <vector>

using ll = long long;

// 1-based；默认全零；sum(x) 始终求当前维护对象的前缀和
struct BIT {
    int n;
    std::vector<ll> bit;

    explicit BIT(int n) : n(n), bit(n + 1, 0) {}

    // 第 x 项加 v；x>=1，x=n+1 时自然跳过；不能传 0
    void add(int x, ll v) {
        for (; x <= n; x += x & -x) bit[x] += v;
    }

    // [1,x] 的和，0<=x<=n；x=0 返回空前缀 0
    ll sum(int x) const {
        ll res = 0;
        for (; x > 0; x -= x & -x) res += bit[x];
        return res;
    }

    // 闭区间 [l,r]，1<=l<=r<=n
    ll sum(int l, int r) const {
        return sum(r) - sum(l - 1);
    }
};
