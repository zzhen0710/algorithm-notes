# 模逆元：素数快速幂与扩展欧几里得

[素数逆元入口](modular_inverse.hpp) · [扩展欧几里得与一般逆元](extended_gcd.hpp)

原理：[7.6 模逆元与组合数取模](../../../notes/07-math/06-modular-inverse-combinations.md) · [7.11 扩展欧几里得与一般模逆元](../../../notes/07-math/11-extended-euclid.md)

## 接口与选用

| 接口 | 输入与前提 | 输出 |
| --- | --- | --- |
| `inv_prime(a,p)` | `a` 任意 ll；调用者保证 `p` 为素数，`2<=p<2^31` | `[0,p)` 中的逆元；`a%p==0` 时返回 `-1` |
| `exgcd(a,b,x,y)` | `0<=a,b<=2e9`；`x、y` 是两个不同变量 | 返回 `gcd(a,b)`，写入一组满足 `a*x+b*y=gcd` 的系数 |
| `inv_general(a,mod)` | `a` 任意 ll；`2<=mod<=2e9`，不要求素数 | `[0,mod)` 中的逆元；不互质时返回 `-1` |

**已知素数可继续用快速幂；一般模数用扩展欧几里得。** 两者都只求一个数的逆元，`-1` 是失败标记，不能继续当作乘数。

`exgcd` 支持零输入，约定 `exgcd(0,0,x,y)` 返回 `0` 并写入 `x=1、y=0`；不接受负输入。`inv_general` 先规范 `a`，因此支持负数，但模数必须至少为 2。

## 使用

```cpp
#include "modular_inverse.hpp" // 同时提供下面三个接口。

ll x, y;
ll g = exgcd(3, 10, x, y); // g=1，x=-3，y=1。
ll a = inv_general(3, 10); // 7；模数是合数也可求。
ll b = inv_general(6, 10); // -1；gcd 不为 1。
ll c = inv_general(-3, 10); // 3；支持负 a。
ll d = inv_prime(2, 5);    // 3；调用者保证模数为素数。
```

只需一般逆元时，可单独包含 `extended_gcd.hpp`，不依赖快速幂。复制为比赛单文件时删去相对 include，先放 `exgcd` 再放 `inv_general`；若还用 `inv_prime`，另保留已有 `mod_pow`。

## 复杂度与边界

- `exgcd`：令 `V=max(a,b,2)`，时间、递归栈空间均为 `O(log V)`；`inv_general` 时间和栈空间均为 `O(log mod)`。
- `inv_prime`：时间 `O(log p)`，空间 `O(1)`；不负责检测素数。
- 三个接口无共享状态，不修改调用者的输入；只有 `exgcd` 写入两个输出引用，多测无需清理。
- `ll` 保存带符号系数与乘法中间值；**本模板不承诺完整 ll 输入范围的裸 exgcd**。扩大范围时重新检查乘法与加减是否溢出。
- 系数不是唯一的，`exgcd` 不保证非负或最小；逆元接口会规范到相应模数范围。
- 合数模数下，阶乘分母可能不可逆；不能仅把组合数模板的 `inv_prime` 换成 `inv_general`。

原 `inv_prime` 的小素数与负数边界历史核验保留；新增一般接口另作 C++11 本地核验。通用接口核验不代替配题评测或个人掌握确认。

[本章模板](../README.md) · [全部模板](../../README.md)
