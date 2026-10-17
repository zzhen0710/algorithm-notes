# gcd/lcm：递归与数组聚合

使用 C++11。实现：[gcd_lcm.hpp](gcd_lcm.hpp)；推导与典题来源见 [7.1 gcd/lcm](../../notes/07-math/01-gcd-lcm.md#7-典题与复盘)。主写法只维护短递归 gcd。

## 接口与约定

- `ll igcd(ll a, ll b)`：接收非负整数，返回 gcd；`igcd(0,0)` 约定为 0。
- `ll ilcm(ll a, ll b)`：接收非负整数，返回 lcm；任意一个输入为 0 时返回 0，**否则要求最终结果不超过 `LLONG_MAX`**。函数不检测超限。
- `ll agcd(const vector<ll>& a)`：逐项求整个数组的 gcd；不修改输入，空数组、全零数组返回 0。
- `ll` 为 `long long`；不接收负数，不通过取绝对值处理 `LLONG_MIN`。
- 两数运算无下标；数组容器为 0-based，容量由元素个数决定。所有函数无共享可变状态，每次调用独立，不需多测清理。

**`a/igcd(a,b)*b` 先除再乘，仍需保证最终结果可表示。** 若任务可能超限，需要先比较商与允许上限，不能直接调用后再判断是否溢出。

## 使用示例

```cpp
#include "gcd_lcm.hpp"
#include <iostream>

int main() {
    std::cout << igcd(48, 18) << '\n'; // 6
    std::cout << ilcm(12, 18) << '\n'; // 36

    std::vector<ll> a = {18, 24, 42};
    std::cout << agcd(a) << '\n'; // 6
    std::cout << ilcm(0, 0) << '\n'; // 0
}
```

## 成本与来源

对正数输入，两数 gcd/lcm 的时间与递归栈空间均为 $O(1+\log\min(a,b))$，零值边界为常数。数组聚合时间上界为 $O(n\log V)$、辅助栈空间 $O(\log V)$，其中 $V=\max(2,\max a_i)$，不计输入容器；空数组为常数。

由 P1029 的 `igcd/ilcm` 与 CF 1618C 的 `agcd` 整理，**为 lcm 增补零值提前返回**，保留短函数与非负数契约。两题原实现留在知识笔记，不覆盖其历史逻辑。模板已完成本地编译与数值边界核验，个人熟练程度及平台结果不由模板生成推断。
