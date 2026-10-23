#pragma once

#include <algorithm>
#include <vector>

using ll = long long;

// a[1..n]，n>=1；静态闭区间最大值，不修改输入
struct SparseMax {
    std::vector<int> lg;
    std::vector<std::vector<ll>> st;

    explicit SparseMax(const std::vector<ll>& a) {
        int n = static_cast<int>(a.size()) - 1;
        lg.resize(n + 1);
        for (int i = 2; i <= n; ++i) lg[i] = lg[i / 2] + 1;

        st.assign(lg[n] + 1, std::vector<ll>(n + 1));
        st[0] = a;
        for (int k = 1; k <= lg[n]; ++k) {
            int len = 1 << k, half = len >> 1;
            for (int i = 1; i + len - 1 <= n; ++i)
                st[k][i] = std::max(st[k - 1][i], st[k - 1][i + half]);
        }
    }

    ll query(int l, int r) const {
        int k = lg[r - l + 1];
        return std::max(st[k][l], st[k][r - (1 << k) + 1]);
    }
};
