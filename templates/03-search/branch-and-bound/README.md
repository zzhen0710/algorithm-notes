# 小规模 0/1 选择：分支限界

实现：[knapsack_bnb.hpp](knapsack_bnb.hpp)，C++11；只依赖标准库，自行整理。

```cpp
#include "knapsack_bnb.hpp"

std::vector<KnapsackItem> items = {{4, 8}, {3, 5}, {2, 3}};
long long answer = knapsack_bnb(items, 5); // 8
```

**用途：** 输入每件物品的重量、价值和容量上限，返回合法子集的最大价值。每件最多一次，空集合法，不要求装满；容量较大、物品数少时可考虑，最坏仍是指数时间。

| 接口／数据 | 约定 |
| --- | --- |
| `KnapsackItem{cost,value}` | 正整数重量、非负整数价值；物品下标 0 到 n−1 |
| `knapsack_bnb(a,cap)` | 容量非负；空输入、容量 0 或全部装不下时返回 0 |
| 数值 | cap、单件 cost、全部 value 之和均不超过 $10^{18}$；中间价值及上界均用 `long long` |
| 初始化／副作用 | 每次调用局部创建 suffix、best；不改输入、无需多测清理、不排序 |
| 返回 | 最优值；不返回选中下标、不统计最优方案数 |
| 复杂度 | 预处理 $O(n)$，搜索最坏 $O(2^n)$，额外空间 $O(n)$，含调用栈 |

`suffix[i]` 是剩余全部价值之和。`value+suffix[i]<=best` 时，整支都无法改善已找到的值，因此停止；等号剪枝适用于只求最优值，不能直接用于统计最优方案数。

**边界：** 不直接适用于恰好装满、恰好选若干件、负重量或其他约束；修改模型后应重新证明初值、答案更新时机与上界。相同物品仍作为不同实体处理，可以分别选择。

通常先从二十件左右评估；25 件已有三千多万种子集，不承诺剪枝后必过。容量较小优先比较 $O(n\cdot\mathrm{cap})$ 的 [0/1 背包 DP](../../../notes/05-dynamic-programming/02-zero-one-knapsack.md)。

完整证明、执行图与三类剪枝见 [3.4 搜索剪枝](../../../notes/03-search/04-search-pruning.md)。P1025／P10483 的个人实现与复盘也在原节维护；其中装箱有独立[通用模板](../bin-packing/README.md)，不把不同模型合进本函数。
