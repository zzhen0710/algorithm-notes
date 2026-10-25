#pragma once

#include <vector>
#include "../modular-arithmetic/modular_inverse.hpp"

// 0<=N<素数p<2^31，内存足够；数组按 0..N，0!=1。
struct ModComb {
    int N;
    ll mod;
    std::vector<ll> fact, ifact;

    explicit ModComb(int n, ll p)
        : N(n), mod(p), fact(N + 1, 1), ifact(N + 1, 1) {
        for (int i = 1; i <= N; ++i) fact[i] = fact[i - 1] * i % mod;
        ifact[N] = inv_prime(fact[N], mod);
        for (int i = N; i; --i) ifact[i - 1] = ifact[i] * i % mod;
    }

    ll C(int n, int k) const { // 0<=n<=N；仅非法 k 返回 0。
        if (k < 0 || k > n) return 0;
        return fact[n] * ifact[k] % mod * ifact[n - k] % mod;
    }
};
