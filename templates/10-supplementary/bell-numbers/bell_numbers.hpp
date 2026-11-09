#pragma once

#include <vector>

// n >= 0，mod 为正 int。返回 B[0..n] 对 mod 的余数。
inline std::vector<long long> bell_numbers(int n, int mod) {
    std::vector<long long> bell(n + 1), comb(n + 1);
    bell[0] = comb[0] = 1 % mod;
    for (int i = 1; i <= n; ++i) {
        for (int k = 0; k < i; ++k)
            bell[i] = (bell[i] + comb[k] * bell[i - 1 - k]) % mod;

        for (int k = i; k >= 1; --k)
            comb[k] = (comb[k] + comb[k - 1]) % mod;
    }
    return bell;
}
