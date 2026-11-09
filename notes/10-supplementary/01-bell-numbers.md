# 10.1 Bell 数与集合划分

给定 $n$ 个可区分元素，求划分成若干非空、无序集合的方案数 $B_n$。按元素顺序逐个塞进组时，给组编号会引入重复计数。改为固定一个元素，通过它所在的组划分情况。

## 1. 预处理代码


```cpp
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
```

**用途速记**　输入上限 `n` 和正整数模数 `mod`，得到从 $B_0$ 到 $B_n$ 的余数。`comb` 保存当前所需的一行组合数。

## 2. 固定一个元素，先确定它所在的组

固定元素 $x$，若它所在的组还有 $k$ 个元素，从其他 $n-1$ 个元素中选出这 $k$ 个的方案数为 $\binom{n-1}{k}$。该组确定后，余下 $n-1-k$ 个元素独立划分，有 $B_{n-1-k}$ 种。

每份划分中，$x$ 所在的组唯一，因此每份划分恰好对应一次选择。对所有 $k$ 相加得

$$
B_n=\sum_{k=0}^{n-1}\binom{n-1}{k}B_{n-1-k}.
$$

$B_0=1$ 表示空集合有一种划分，即零个组。三个可区分元素的划分数是 $B_3=2+2+1=5$，对应 $x$ 所在组的大小分别为 1、2、3。

## 3. 先使用组合数行，再推进

进入第 `i` 轮时，`comb[k]` 是 $\binom{i-1}{k}$ 的余数。先用这行求 `bell[i]`，再倒序推进到第 `i` 行。已有 Bell 数全部保留，因为新一项可能用到任一更小下标。时间 $O(n^2)$，空间 $O(n)$。组合数由加法得到，模数可以是合数；正 `int` 模数下，用 `long long` 计算乘积及其与余数的和。

元素按身份区分，属性或数值相同仍可能是不同元素；同一划分中组和组内元素都不计顺序。

[杨辉三角滚动](../07-math/05-basic-counting.md#一维滚动只保留当前行) · [CF 2283E 应用](../../contests/codeforces/2283.md#e-partition-of-a-binary-string) · [竞赛模板](../../templates/10-supplementary/bell-numbers/README.md)
