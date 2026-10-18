#pragma once

#include <utility>
#include <vector>

using ll = long long;

// 接受任意 ll；负数、0、1 返回 false；最坏 O(sqrt(n))，不适合大质数
inline bool is_prime(ll n) {
    if (n < 2) return false;

    for (ll d = 2; d <= n / d; ++d) {
        if (n % d == 0) return false;
    }

    return true;
}

// 0<=N<INT_MAX，且内存足够；返回 0-based、覆盖 [0,N] 的埃氏筛表
inline std::vector<bool> sieve(int N) {
    std::vector<bool> prime(N + 1, true);
    prime[0] = false;
    if (N >= 1) prime[1] = false;

    for (int p = 2; p <= N / p; ++p) {
        if (!prime[p]) continue;

        for (ll j = 1LL * p * p; j <= N; j += p)
            prime[j] = false; // 倍数递增也用 ll，避免末次加法溢出
    }

    return prime;
}

// 与 sieve 相同的范围和返回值；欧拉筛 O(N)，每个合数只生成一次
inline std::vector<bool> linear_sieve(int N) {
    std::vector<bool> prime(N + 1, true);
    std::vector<int> primes;
    prime[0] = false;
    if (N >= 1) prime[1] = false;

    for (int i = 2; i <= N; ++i) {
        if (prime[i])
            primes.push_back(i); // 新质数先入表，平方也能在本轮生成

        for (int p : primes) {
            if (p > N / i) break; // 通过范围检查后再计算乘积

            prime[i * p] = false; // p 不大于 i 的最小质因子

            if (i % p == 0)
                break; // 先生成等号情形，再排除更大的 p
        }
    }

    return prime;
}

// n>=1；按质因子递增返回 (质因子,指数)，n=1 返回空分解
inline std::vector<std::pair<ll, int>> factorize(ll n) {
    std::vector<std::pair<ll, int>> factors;

    for (ll p = 2; p <= n / p; ++p) {
        if (n % p != 0) continue;

        int e = 0;
        while (n % p == 0) {
            n /= p; // 除尽当前质因子，再检查下一候选
            ++e;
        }

        factors.emplace_back(p, e);
    }

    if (n > 1) factors.emplace_back(n, 1);
    return factors;
}
