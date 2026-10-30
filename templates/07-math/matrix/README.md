# 矩阵乘法与快速幂

实现：[matrix.hpp](matrix.hpp)。默认 C++11，`Mat` 为 `vector<vector<long long>>`，下标从 0 开始。

| 接口 | 输入与结果 | 时间 |
| --- | --- | --- |
| `mat_mul(a, b, mod)` | A 为 n×r、B 为 r×m，返回 n×m 的模意义乘积 | $O(nrm)$ |
| `mat_pow(a, k, mod)` | 非空方阵、非负 `long long k`，返回 A 的 k 次幂 | $O(d^3\log(k+1))$，k=0 时 $O(d^2)$ |

**输入约定：** 矩阵非空、各行长度一致、维度相容，模数满足 $1\le\mathrm{mod}\lt2^{31}$。`mat_mul` 输入元素须在 `[0,mod)`；`mat_pow` 先规范任意 `long long` 元素，包含负数。模数不必为素数。

```cpp
#include "matrix.hpp"

Mat a = {{2, 1}, {1, 3}};
long long mod = 1000000007;
Mat power = mat_pow(a, 5, mod);

// 列向量：先求转移矩阵的幂，再作用于初始状态。
Mat initial = {{1}, {0}}; // 单独调用 mat_mul，须保证元素已规范。
Mat state = mat_mul(power, initial, mod);
```

**边界与副作用：** k=0 返回单位矩阵，mod=1 返回全 0 余数；输入对象均不被修改，每次调用新建结果，无需额外清理。非方阵、空输入、非法维度、负指数不支持，不做运行时校验。

**空间与数值：** 乘法额外空间 $O(nm)$，快速幂额外空间 $O(d^2)$。逐项取模，在约定模数内 `long long` 乘加安全；扩大模数必须另处理溢出。普通乘法不直接处理 `max/min` 转移，矩阵顺序也不能任意交换。

状态与指数的推导见[7.9 矩阵与矩阵快速幂](../../../notes/07-math/09-matrix-exponentiation.md)；组合思想见[统一变换，按顺序复合](../../../patterns/09-data-structures/composable-transformations.md#同一种线性变换重复很多次矩阵快速幂)。递推式怎样构造系数与初始状态，见[递推矩阵构造范式](../../../patterns/07-math/recurrence-to-matrix.md)。`power` 是幂矩阵，显式乘初始列向量后的 `state` 才是最终状态；答案按目标分量读取。本组继续复用同一份通用运算，不增加专属数列函数。

[返回本章模板](../README.md)
