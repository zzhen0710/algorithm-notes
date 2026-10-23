# 全图二分图染色与判定

BFS 传播异色约束，外层遍历全部连通块。原理、边界与个人题目复盘见 [6.2 二分图染色与判定](../../../notes/06-graph-and-tree/02-bipartite-coloring.md#5-通用实现)。使用 C++11。

实现：[bipartite_coloring.hpp](bipartite_coloring.hpp)。

## 接口与约定

- `bool bipartite_color(const vector<vector<int>>& g, vector<int>& color)`：全图可二染色返回 `true`，否则返回 `false`。
- **点编号 1-based**；`g` 至少有一个留空的 `g[0]`，外层长度可用 `int` 表示，每个邻居编号在 `1..n`。
- 输入为无向邻接表，**每条边必须存两个方向**。孤立点与重边合法，自环直接产生冲突。
- 函数不修改 `g`，每次调用覆盖并初始化 `color`；`-1` 表示未染色，`0、1` 是有效颜色，`color[0]` 保持 `-1`。
- 成功时全部点已染色；失败时可能只完成部分染色，不能据此输出合法方案。
- 没有顶点时传入长度为 `1` 的邻接表，返回 `true`；不要传缺少下标 `0` 的空容器。

## 使用示例

```cpp
#include "bipartite_coloring.hpp"
#include <iostream>

int main() {
    std::vector<std::vector<int>> g = {{}, {2}, {1}, {}};
    std::vector<int> color;

    bool ok = bipartite_color(g, color);
    std::cout << ok << '\n';        // 1，点 1、2 异色
    if (ok) std::cout << color[3] << '\n'; // 0，孤立点也得到颜色
}
```

## 边界、成本与来源

**同一张图只初始化一次颜色表，各连通块共享它；独立调用会重新初始化。** 不能把颜色 `0` 当未访问，也不能在已染色时直接跳过约束检查。

时间 $O(n+m)$，辅助空间 $O(n)$；算上调用方的邻接表存储，总空间 $O(n+m)$。各块初始颜色无全局选取含义，不能用全图两色人数较小值代替逐块最优化，见 [P1330 的独立选择论证](../../../notes/06-graph-and-tree/02-bipartite-coloring.md#为什么逐块取小能得到全局最小)。

从现有通用函数提取，仅补自包含头文件、`std::` 与 `inline`，不改变算法。模板已完成本地编译、逐掩码染色对照及边界核验；配套题平台 AC 依据来源笔记中的本人反馈，不能据此给模板伪造平台提交记录。

[本章模板](../README.md) · [全部模板](../../README.md)
