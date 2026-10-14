# 整数二分：可行边界与独立判定

适用：整数候选的判定结果单调。使用 C++11，头文件通过 `using ll = long long;` 定义整数类型别名，候选范围为 $0\le L\le R\le10^{18}$。

实现见 [binary_search.hpp](binary_search.hpp)，原理与来源见[2.6 二分答案](../../notes/02-sequence-techniques/06-binary-search-on-answer.md)，选型见[独立判定范式](../../patterns/binary-search-on-answer.md)。

## 接口与约定

| 接口 | 判定方向 | 必须保证 | 返回 |
| --- | --- | --- | --- |
| `firstTrue(L,R,check)` | `false → true` | `check(R)` 为真 | 最小可行整数 |
| `lastTrue(L,R,check)` | `true → false` | `check(L)` 为真 | 最大可行整数 |

两个接口采用同一套闭区间收缩规则，仅中点方向与更新方向不同。**调用前必须保证范围内有解**；接口不会自动检查这一前提。

## 独立判定的用法

`check` 可以是函数或 lambda。常用结构是 `auto check = [&](ll limit) -> bool { ... };`：`[&]` 引用捕获已有数据，`limit` 是本次候选，返回值表示是否可行。频次、累计量等临时状态在每次调用内部初始化。语法拆解与待填写骨架见[范式中的判定写法](../../patterns/binary-search-on-answer.md#判定-lambda-怎么写)。

```cpp
#include "binary_search.hpp"
#include <iostream>

int main() {
    // 最小可行值：测试范围中从 6 开始可行
    ll need = 6;
    auto check = [&](ll limit) -> bool {
        return limit >= need; // 引用外部目标，判断一次候选
    };
    std::cout << firstTrue(0, 10, check) << '\n'; // 6

    // 最大可行值：测试范围中到 7 为止可行
    ll bound = 7;
    auto allowed = [&](ll limit) -> bool {
        return limit <= bound;
    };
    std::cout << lastTrue(0, 10, allowed) << '\n'; // 7
}
```

> **模板负责寻找边界，lambda 负责判断一次候选。模板正确不能代替判定正确。**

## 边界与初始化

- 最小可行值可能无解时，先检查 `check(R)`；最大可行值检查 `check(L)`，失败后按题意处理。
- 单点范围合法；全体候选可行时，分别返回左端或右端。
- 候选 `0` 必须对判定有意义；包含除法时尤其要检查，不合法则从正数范围搜索。
- `check` 必须保持单调，并且对同一候选返回一致结果；多组数据重新设置捕获的数据与边界。
- `ll` 不能挽救先发生的 `int` 运算溢出；累加、乘法和上界计算分别检查。
- 当前范围保证中点差值与向上取整安全，不支持直接传入整个有符号 64 位范围。

若单次判定为 $C$，候选数为 $W=R-L+1$，总时间为 $O(C(1+\log W))$；搜索自身额外空间为 $O(1)$，另计判定所需空间。

## 验证状态

已完成本地编译、两种单调方向、单点/相邻候选、左右端答案、全体可行及大数边界核验；上面的使用示例已编译运行。

代码验证与个人独立复现状态分别记录，不以模板存在推断已经熟练。
