# 树形 DP 基础模型：连通点集与独立集

实现：[tree_dp.hpp](tree_dp.hpp)。来源：[5.7 两题复盘](../../../notes/05-dynamic-programming/07-tree-dp.md#8-典题与复盘)，建模见 [固定节点状态范式](../../../patterns/05-dynamic-programming/subtree-boundary-state.md)。

| 函数 | 返回值与状态 | 空方案 |
| --- | --- | --- |
| max_connected_sum(g,w,root=1) | 最大连通点集权值；dp[u] 必须包含 u | 不允许，单点／全负时仍选一个节点 |
| max_tree_independent_set(g,w,root=1) | 最大权独立集，相邻点不能同时选；按 u 不选／选分状态 | 允许，全负时返回 0 |

## 前提与依赖

- **图：** 1-based 非空连通无向树，g 长度 n+1、g[0] 留空，每条边双向存储；无自环、重边或环。root 合法，不校验输入结构。
- **权值：** w 长度 n+1，允许负、零、正值；所有中间和都须可用 ll 表示，包括非最终最优状态。两个函数均返回 ll。
- **状态范围：** 不附加必须选某个点、恰好选 k 个、共享预算等条件；这些约束需要修改状态，不能直接调用。
- **依赖：** 复用 [dfs_order.hpp](../../06-graph-and-tree/dfs-order/dfs_order.hpp)。复制成单文件时先展开 DFSOrder/tree_order，再放所需函数，删去本地相对 include。
- **副作用与多测：** 不修改 g、w，每次调用独立创建遍历结果和 DP；修改图或权值后重新调用。根只组织递推，不改变这两个无向树模型的最优值。
- **复杂度：** 每个函数时间 O(n)、额外空间 O(n)，包含非递归遍历。连续调用两函数会分别遍历一次。

## 最小使用

```cpp
#include "tree_dp.hpp"

std::vector<std::vector<int>> g(4);
g[1] = {2};
g[2] = {1, 3};
g[3] = {2};
std::vector<ll> w = {0, 100, 10, 9};

ll connected = max_connected_sum(g, w); // 119，三个点都保留
ll independent = max_tree_independent_set(g, w); // 109，选 1、3
```

这两份实现分别提炼自 P1122 与 P1352，使用 ll，并共用现有无向树遍历；与 P1352 原实现只存孩子、按输入找上司根的方式不同。传入模板时先满足本页的无向图契约。

本地核验通过，包含小树子集枚举、负权／零权、不同根、长链与 ll 量级；不记为平台 AC。具体题目过程留在 Notes，不重复为每道题创建模板组。

[本章模板](../README.md) · [全部模板](../../README.md)
