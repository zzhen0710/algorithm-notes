# 差分约束：统一不等式、判定与一组解

[知识笔记与方向图](../../../notes/06-graph-and-tree/11-difference-constraints.md) · [实现](difference_constraints.hpp)

通过一个薄封装复用[负权最短路模板](../negative-shortest-path/README.md)，不复制 SPFA 主体。点对应变量，接口直接接收变量差条件。

## 接口与约定

```cpp
DifferenceConstraints dc(n);
dc.add_le(i, j, c); // x[i]-x[j] <= c
dc.add_ge(i, j, c); // x[i]-x[j] >= c
dc.add_eq(i, j, c); // x[i]-x[j] == c
auto res = dc.solve();
```

**用途速记：** 把每条变量差限制加入系统，全图检查是否冲突。negative_cycle=true 表示无解；否则 res.dist[1..n] 是一组满足全部限制的解，不自动代表变量总和最小。

| 部分 | 职责／返回 | 单次开销 |
| --- | --- | --- |
| 构造函数 | n 个变量，邻接表开 n+1 项 | O(n) 时间与空间 |
| add_le(i,j,c) | j→i，权 c | 均摊 O(1)，增加一条边 |
| add_ge(i,j,c) | i→j，权 −c | 均摊 O(1)，增加一条边 |
| add_eq(i,j,c) | 同时加入两个方向 | 均摊 O(1)，增加两条边 |
| solve() | 调用全图模式 SPFA，返回 negative_shortest::Result | 最坏 O(nM+n)，O(n) 辅助空间 |

- **编号与条件：** 1-based；n≥1，n+1、实际有向边数 M 可用 int 表示且内存足够。只支持变量差上界／下界／等式；不直接表达乘积、一般系数线性不等式或最优化目标。
- **权重范围：** 令 W 为实际边权绝对值上限，要求 nMW≤10¹⁸；没有边时不产生加法。c 取负和整数严格不等式中 c−1 的运算也必须安全。不要先在整数类型中算出溢出的乘积再比较。
- **等式与 M：** add_eq 计两条实际边，估算复杂度与范围时也要计两条。自我限制、重边均可保留。
- **初始化：** solve 使用全部点距离为 0、全部点入队，不局限于变量 1 的约束分量；这里的 0 是接口约定，不是真实变量编号。
- **解与标志：** 无负环时返回整数可行解，一般可含负数；有负环时 dist 仅是中间结果，不能继续输出成解。返回结果不是最小总和或最少资源的自动答案。
- **绝对条件：** 统一平移不会改变变量差，但固定变量值、不同的额外上下界等还需单独建模；本类不自动处理锚点。只有相对差值系统时，全零初值不会改变可行性。
- **副作用与多测：** 添加条件会改变 g；solve() 不改变图，每次重建算法状态。新测试用例新建对象，返回的 dist 独立保存，不依赖对象后续生命周期。
- **空间：** 图 O(n+M)，solve 的状态／队列／返回值 O(n)。
- **依赖：** C++11；头文件相对包含 `../negative-shortest-path/negative_shortest_path.hpp`。复制单文件参赛时，把该依赖及本结构体一并放入源文件。

## 使用示例

```cpp
#include "difference_constraints.hpp"
#include <iostream>

int main() {
    DifferenceConstraints dc(3);
    dc.add_le(2, 1, 3);
    dc.add_le(3, 2, -2);
    dc.add_le(1, 3, 1);
    auto res = dc.solve();
    if (res.negative_cycle) std::cout << "NO\n";
    else {
        for (int i = 1; i <= 3; ++i) std::cout << res.dist[i] << ' ';
        std::cout << '\n'; // 此遍历顺序得到 -1 0 -2
    }
}
```

本类只做建边和调用，解的正确性依据见笔记的[负环矛盾](../../../notes/06-graph-and-tree/11-difference-constraints.md#4-为什么负环代表矛盾沿约束代入一圈)与[无负环时的可行解](../../../notes/06-graph-and-tree/11-difference-constraints.md#6-为什么没有负环就一定能返回解)。P5960／P1993 的实际建边与返回值使用已在[典题复盘](../../../notes/06-graph-and-tree/11-difference-constraints.md#9-典题与复盘)中整理；模板继续只维护通用接口，不混入题目操作编号或 solve。

[本章模板](../README.md) · [全部模板](../../README.md)
