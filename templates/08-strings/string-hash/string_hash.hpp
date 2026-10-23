#pragma once

#include <string>
#include <vector>
#include <utility>
#include <tuple>

using ll = long long;

// 静态字节串双哈希；长度及 n+1 可用 int 表示，查询为合法 0-based [l,r)
// 固定参数不能保证无碰撞；不保存原文，修改原串后须重建
struct StringHash {
    enum {
        BASE = 911382323,
        MOD1 = 1000000007,
        MOD2 = 1000000009
    };

    using Key = std::tuple<int, ll, ll>;

    int n;
    std::vector<ll> h1, h2, pw1, pw2;

    explicit StringHash(const std::string& s)
        : n(static_cast<int>(s.size())),
          h1(n + 1), h2(n + 1), pw1(n + 1), pw2(n + 1) {
        pw1[0] = pw2[0] = 1;

        for (int i = 0; i < n; ++i) {
            int c = static_cast<unsigned char>(s[i]) + 1;
            h1[i + 1] = (h1[i] * BASE + c) % MOD1;
            h2[i + 1] = (h2[i] * BASE + c) % MOD2;
            pw1[i + 1] = pw1[i] * BASE % MOD1;
            pw2[i + 1] = pw2[i] * BASE % MOD2;
        }
    }

    std::pair<ll, ll> get(int l, int r) const {
        ll x = (h1[r] - h1[l] * pw1[r - l] % MOD1 + MOD1) % MOD1;
        ll y = (h2[r] - h2[l] * pw2[r - l] % MOD2 + MOD2) % MOD2;
        return {x, y};
    }

    Key key(int l, int r) const {
        auto h = get(l, r);
        return {r - l, h.first, h.second}; // 自动保留长度，仍只是指纹键
    }
};
