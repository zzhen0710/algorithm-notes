# 负权最短路与负环：Bellman–Ford、SPFA

[知识笔记、过程图与证明](../../../notes/06-graph-and-tree/10-negative-shortest-path.md) · [实现](negative_shortest_path.hpp)

## 接口与输入

所有类型和函数位于 `negative_shortest` 命名空间；使用 C++11、`using ll = long long;`。

| 接口／类型 | 含义 |
| --- | --- |
| `Edge{u,v,w}` | 一条有向边；供边表使用 |
| `AdjEdge{v,w}` | 邻接表中从当前点到 v 的有向边 |
| `bellman_ford(n,edges,s=1)` | 扫描完整边表，返回距离与负环标志 |
| `spfa(g,s=1)` | 队列安排传播；g.size()=n+1 |
| `Result.dist` | 距离数组，有效下标 1..n；标志为 false 时才可作为最终结果 |
| `Result.negative_cycle` | 初始化所能到达的范围中是否有负环 |

- **编号：** 1-based，n≥1；普通单源要求 `1 <= s && s <= n`。参数 `s=0` 是全图模式，等价于虚拟源以 0 权边连向所有点，不查询或遍历真实点 0。
- **图：** 可不连通，允许自环、重边；无向关系由调用者存双向边。m 指实际有向边数量，不是必然等于题目输入记录数。
- **数值契约：** n≥1，n+1、实际有向边数 m 可用 int 表示且内存足够；令 W 为边权绝对值上限，要求数学意义上的 n×m×W≤10¹⁸。BF 至多 n×m 次检查，检测期间的走法代价仍安全落在 ll 与 INF 的区间内；SPFA 以走法边数达到 n 截止，也在此范围内。没有边时不产生边权加法。先作数学估算或用除法比较，不能把乘积先算溢出再判断。教学正文的 n≤2000、m≤20000、W≤10⁹ 是满足该条件的一组具体范围，不是算法只能处理这么多点。
- **初始化：** 单源时仅起点为 0，其他为 INF；全图模式所有点为 0，SPFA 还会将全部点入队。每次调用重建本地状态，无多测残留。
- **负环时的结果：** 函数立即结束，dist 是中间值，不标记哪些点应为负无穷，也不提供负环路径；本接口不恢复路径。
- **不可达：** 普通单源中保持 INF；仅在负环标志为 false 时读取完整结果。全图模式无负环时，dist 是虚拟源最短距离，不是原图某个固定点的距离。
- **SPFA 标记：** inq 只表示当前是否在队列，出队即清；hops 是当前更新走法的边数，不是累计改善次数。不要将二者改成永久 vis 或直接自增的松弛计数。
- **复杂度：** 两者最坏时间 O(nm+n)，常简写 O(nm)，辅助空间 O(n)，不含输入图；SPFA 不承诺线性复杂度。边表本身 O(m)，邻接表本身 O(n+m)。
- **依赖与副作用：** 仅标准头文件，输入只读，无全局可变状态；返回对象保存自己的距离副本，不依赖原图后续生命周期。

[差分约束封装](../difference-constraints/README.md)复用本实现的全图模式，约束建边与数值范围须由调用者检查；本算法不判断输入边是否正确表达了题意。

## 使用示例

```cpp
#include "negative_shortest_path.hpp"
#include <iostream>

int main() {
    using namespace negative_shortest;
    std::vector<Edge> edges = {{1, 2, 2}, {2, 4, 1},
                               {1, 3, 5}, {3, 2, -10}};
    auto a = bellman_ford(4, edges, 1);

    std::vector<std::vector<AdjEdge>> g(5);
    for (const auto& e : edges) g[e.u].push_back({e.v, e.w});
    auto b = spfa(g, 1);
    std::cout << a.negative_cycle << ' ' << a.dist[4] << '\n'; // 0 -4
    std::cout << b.negative_cycle << ' ' << b.dist[4] << '\n'; // 0 -4
}
```

**用途速记：** 输入有向图和起点，反复传播距离改善，允许负边并检测可达负环；普通单源检测范围由起点决定，全图模式用于检查任何连通部分。先判断标志，再读距离，不能把检测时的中间结果当成最短路。

算法条件参考 [CP-Algorithms](https://cp-algorithms.com/graph/bellman_ford.html)，SPFA 采用[OI Wiki](https://oi-wiki.org/graph/shortest-path/)所述的传播走法边数判定。代码按本仓库契约自行整理，未直接复制上游实现。两题个人判环版返回 bool，具体 solve 与范围对照见[典题复盘](../../../notes/06-graph-and-tree/10-negative-shortest-path.md#7-典题与复盘)；P3385 当前贴稿的 s=0 与原题单源范围有差别，修正调用单列，P2850 的 s=0 符合全图判环。本轮未重复运行题目专属评测，通用模板无需改动。

[本章模板](../README.md) · [全部模板](../../README.md)
