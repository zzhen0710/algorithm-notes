# 分组背包：每组至多 / 恰好选一

实现：[group_knapsack.hpp](group_knapsack.hpp)。推导、读写图与典题复盘见 [5.12 分组背包](../../../notes/05-dynamic-programming/12-group-knapsack.md)。自行按旧组转移整理，不依赖其他模板。

```cpp
#include "group_knapsack.hpp"

std::vector<std::vector<GroupItem>> groups{
    {{2, 6}, {3, 7}},
    {{2, 5}, {1, 3}}
};
ll ans = group_knapsack(4, groups); // 11，每组至多选一，总占用不超过 4。

std::vector<std::vector<GroupItem>> required{{{2, -3}}, {{1, 0}}};
ll a = group_knapsack(4, required, true); // -3，允许剩余容量。
ll b = group_knapsack(2, required, true); // GROUP_NEG，无解。
```

**用途速记：** 输入已经分好的互斥候选，返回容量上限内的最大价值。组、容量倒序、组内候选三层循环；新结果暂存 best，比较完当前容量的候选后才写回。

| 接口 / 字段 | 含义 |
| --- | --- |
| `GroupItem {cost, value}` | 一个候选的占用与价值，不能用数量上限解释 |
| `groups[g]` | 第 g 组候选；同组最多采用一种 |
| `group_knapsack(cap, groups)` | 每组至多选一；可全部不选，答案至少为 0 |
| `group_knapsack(cap, groups, true)` | 每组恰好选一；无解返回 `GROUP_NEG` |

## 前提与边界

- 全部数组 0-based，dp 下标为 `0..cap`；调用者负责把组号映射为实际 groups，不把编号空隙当作必须选的空组。
- `0<=cap<=200000`，实际组数和候选总数均不超过 200000；cost 为非负 int，value 在 `[-1e12,1e12]`。总价值绝对值不超过 `2e17`，与 `GROUP_NEG=-(1LL<<60)` 分离；遇到哨兵先跳过，不做加法。
- **若全部 cost>0，可以省去 best，容量倒序后直接更新 dp[j]。** 这是较窄条件下的简写；当前模板保留 best，兼容零费用与必选模式。
- **支持 cost=0**：本格候选都读取还没覆盖的旧 dp[j]，best 只在组内循环结束后写回。不要改成在组内反复覆盖 dp[j]，否则同组零占用候选可能叠加。
- 空 groups 返回 0；容量 0 仍会处理零占用候选；cost 超出当前容量自动跳过。存在空组时，默认模式跳过；恰好选一模式无解。
- 必选模式允许负的最优值，只有等于 `GROUP_NEG` 才表示无解。它要求每组恰好一件，**不要求恰好装满容量**。
- 输入只读，每次调用新建局部 dp，多测无需全局清理；接口不返回完整表，不记录选法，不支持计数、跨组依赖或多维资源。
- `ll`、`GroupItem`、`GROUP_NEG` 为头文件级名字，现场合并时不要重复定义；可与已有 `BoundedItem` 共存。

## 复杂度与核验

G 为组数、M 为候选总数、C 为容量。时间 O((C+1)(G+M)+C+1)，非空组时常简写 O(CM)；额外空间 O(C)，输入存储 O(G+M)。cap 与候选数的类型边界不是时间保证，应先估算其乘积。

教学旧行版和一维模板分别与小规模逐组选法枚举核对，覆盖零占用、负价值、空组、容量不足与重复调用。P1757 已收到个人实现并静态复盘，补充正费用简写、零费用及组号范围的区别；未重复评测个人代码，通用验证不替代平台结果或个人掌握。

[本章模板](../README.md) · [背包建模范式](../../../patterns/05-dynamic-programming/resource-limited-selection.md)
