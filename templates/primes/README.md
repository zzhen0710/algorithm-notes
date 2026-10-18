# 素数：判定、埃氏筛与质因数分解

使用 C++11。实现：[prime_algorithms.hpp](prime_algorithms.hpp)；原理及两题复盘见 [7.4 素数与分解](../../notes/07-math/04-primes-sieve-factorization.md)。

**模板以已实际练习的埃氏筛为主。** [欧拉筛的唯一生成规则](../../notes/07-math/04-primes-sieve-factorization.md#欧拉筛线性筛让每个合数只生成一次)留在笔记中对照，不同时维护第二套筛法模板。

## 接口与约定

- `bool is_prime(ll n)`：判断是否为质数；接受任意 `ll`，小于 2 时返回 `false`。最坏时间 $O(\sqrt n)$，辅助空间 $O(1)$；**大质数即使用 `ll` 表示，也可能无法在时限内试除完**。
- `vector<bool> sieve(int N)`：返回闭区间 `[0,N]` 的质数表，**0-based，长度 `N+1`，0、1 为 `false`**。要求 `0<=N<INT_MAX` 且内存足够；查询前保证下标在表内。对 $N\ge3$，时间 $O(N\log\log N)$、空间 $O(N)$，之后单次查询 $O(1)$；小范围常数处理。
- `vector<pair<ll,int>> factorize(ll n)`：要求 **`n>=1`**，每项为“质因子、指数”，质因子严格递增，指数为正；1 返回空分解。最坏时间 $O(\sqrt n)$，不计返回容器时辅助空间 $O(1)$，结果空间上界 $O(\log n)$；1 为常数处理。
- `ll` 为 `long long`。试除使用 `d<=n/d`，筛选倍数用 `ll`；不先计算可能溢出的平方。
- 参数按值接收，不修改调用处输入；状态和返回容器每次重新初始化，**无共享可变状态，多测无需清理**。同一值域反复查询时，在调用处保留筛表复用，不必每次重建。

> **筛表按最大查询值分配，分解结果按不同质因子记录。** 前者保存每个数的真假，后者保存一个数的乘法结构，不能互相替代。

## 使用示例

```cpp
#include "prime_algorithms.hpp"
#include <iostream>

int main() {
    std::cout << is_prime(97) << '\n'; // 1

    auto prime = sieve(20);
    std::cout << prime[2] << ' ' << prime[9] << '\n'; // 1 0

    auto factors = factorize(360);
    for (const auto& item : factors)
        std::cout << item.first << '^' << item.second << ' ';
    std::cout << '\n'; // 2^3 3^2 5^1
}
```

## 来源与边界

从 7.4 的通用实现提炼，P5736 原稿的 `sieze` 命名在模板中规范为 `sieve`；模板使用 `ll` 保存倍数，原稿按原样留在复盘。

CF 1294C 只需寻找满足条件的三个整数，不需要完整质因数分解；**不把该题专属构造包装成通用模板**。模板已完成本地编译、数值边界和重复调用核验，平台 AC 与个人掌握情况仍按实际记录。
