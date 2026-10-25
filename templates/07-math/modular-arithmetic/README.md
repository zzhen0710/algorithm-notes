# 模逆元：素数模数下的快速幂

实现：[modular_inverse.hpp](modular_inverse.hpp)。原理见 [7.6 模逆元与组合数取模](../../../notes/07-math/06-modular-inverse-combinations.md)。

- **接口：** inv_prime(a,p)，a 可为任意 ll；调用者保证 p 为素数，且 2≤p<2³¹。
- **返回值：** 存在时返回 0..p-1 中的逆元；a%p=0 时返回 -1，调用者应先检查，不能把 -1 当作正常结果参与乘法。
- **前提：** 不检测素数，不适用于一般合数模数；p=1 不合法。逆元存在性一般要求 gcd(a,p)=1，本函数利用了 p 已是素数。
- **复杂度：** 时间 O(log p)、空间 O(1)；局部初始化，无共享状态，不修改参数来源，多测无需清理。
- **依赖：** 自动包含同章 [binary_exponentiation.hpp](../binary-exponentiation/binary_exponentiation.hpp)；复制成单文件时先放 mod_pow，再放 inv_prime，删除本地相对 include。

```cpp
#include "modular_inverse.hpp"

ll x = inv_prime(2, 5);  // 3
ll y = inv_prime(-2, 5); // 2；先将 -2 规范为 3
ll bad = inv_prime(5, 5); // -1，不存在
```

标准 ll 取模和乘法即可覆盖所述范围；不扩大模数契约，也不引入不需要的通用模数封装。

**C++11 本地核验通过。** 与小素数下逐个枚举逆元对照，检查负数、0、素数倍数、p=2 与大模数；通用核验不代表配题通过。

[本章模板](../README.md) · [全部模板](../../README.md)
