# 素数：判定、两种筛法与质因数分解

使用 C++11。实现：[prime_algorithms.hpp](prime_algorithms.hpp)；原理及两题复盘见 [7.4 素数与分解](../../../notes/07-math/04-primes-sieve-factorization.md)。

提供埃氏筛 `sieve` 与欧拉筛 `linear_sieve` 两种算法，**接口一致，按需求选其中一种建表即可**。欧拉筛的数学依据、循环变量含义及实例见[第 4 节](../../../notes/07-math/04-primes-sieve-factorization.md#4-欧拉筛由最小质因子唯一生成合数)。

## 接口与约定

- `bool is_prime(ll n)`：判断是否为质数；接受任意 `ll`，小于 2 时返回 `false`。最坏时间 $O(\sqrt n)$，辅助空间 $O(1)$；**大质数即使用 `ll` 表示，也可能无法在时限内试除完**。
- `vector<bool> sieve(int N)`：返回闭区间 `[0,N]` 的质数表，**0-based，长度 `N+1`，0、1 为 `false`**。要求 `0<=N<INT_MAX` 且内存足够；查询前保证下标在表内。对 $N\ge3$，时间 $O(N\log\log N)$、空间 $O(N)$，之后单次查询 $O(1)$；小范围常数处理。
- `vector<bool> linear_sieve(int N)`：范围、下标与返回值同 `sieve`。使用已发现的递增质数列表，让每个合数仅由最小质因子生成一次；时间 $O(N)$、空间 $O(N)$，建表后单次查询 $O(1)$，0、1 的小范围为常数处理。
- `vector<pair<ll,int>> factorize(ll n)`：要求 **`n>=1`**，每项为“质因子、指数”，质因子严格递增，指数为正；1 返回空分解。最坏时间 $O(\sqrt n)$，不计返回容器时辅助空间 $O(1)$，结果空间上界 $O(\log n)$；1 为常数处理。
- `ll` 为 `long long`。试除使用 `d<=n/d`，埃氏筛的倍数用 `ll`；欧拉筛先检查 `p<=N/i`，再计算表内乘积，不先做可能溢出的乘法。
- 参数按值接收，不修改调用处输入；状态和返回容器每次重新初始化，**无共享可变状态，多测无需清理**。同一值域反复查询时，在调用处保留筛表复用，不必每次重建。

> **筛表按最大查询值分配，分解结果按不同质因子记录。** 前者保存每个数的真假，后者保存一个数的乘法结构，不能互相替代。

欧拉筛中，`i` 遍历所有整数，合数轮次也要执行；`i%p==0` 时必须**先标记 `i*p` 再退出内层**。不能照搬埃氏筛跳过合数基数的逻辑，也不能只因理论线性就保证实测一定更快。

## 使用示例

```cpp
#include "prime_algorithms.hpp"
#include <iostream>

int main() {
    std::cout << is_prime(97) << '\n'; // 1

    auto prime = sieve(20);
    std::cout << prime[2] << ' ' << prime[9] << '\n'; // 1 0

    auto linear = linear_sieve(20);
    std::cout << (prime == linear) << '\n'; // 1：两种算法返回相同的真假表

    auto factors = factorize(360);
    for (const auto& item : factors)
        std::cout << item.first << '^' << item.second << ' ';
    std::cout << '\n'; // 2^3 3^2 5^1
}
```

## 来源与边界

从 7.4 的通用实现提炼，P5736 原稿的 `sieze` 命名在模板中规范为 `sieve`；埃氏筛使用 `ll` 保存倍数，欧拉筛在乘法前以 `p>N/i` 检查范围，原稿按原样留在复盘。

CF 1294C 只需寻找满足条件的三个整数，不需要完整质因数分解；**不把该题专属构造包装成通用模板**。模板已完成本地编译、数值边界和重复调用核验，平台 AC 与个人掌握情况仍按实际记录。

[本章模板](../README.md) · [全部模板](../../README.md)
