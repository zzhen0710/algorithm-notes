# 无向图割点与桥：显式 DFS

[知识笔记与执行图](../../../notes/06-graph-and-tree/13-articulation-points-bridges.md) · [实现](cuts.hpp)

```cpp
#include "cuts.hpp"

UndirectedGraph g(n);
int eid = g.add_edge(u, v); // 一次加入一条无向边，内部双向存储
auto res = find_cuts(g);
bool is_cut = res.cut[u];
bool is_bridge = res.bridge[eid];
auto endpoints = g.edges[eid];
```

**用途速记：** 输入无向图，返回删除后会增加连通块数量的点和边。逐个尝试删除的搜索，被 DFS 的回连信息与返回时的比较代替；保留原边编号，结果可以直接映射回输入端点。

| 部分 | 约定 | 开销 |
| --- | --- | --- |
| UndirectedGraph(n) | 真实点 1..n，g[0] 不使用 | O(n) 时间、空间 |
| add_edge(u,v) | 返回无向边编号 0..m−1 | 均摊 O(1)，加入两条邻接记录 |
| find_cuts(g) | 返回 cut 与 bridge，不修改图 | O(n+m) 时间，O(n+m) 结果及辅助空间 |
| res.cut[u] | 点 u 是否为割点 | 数组 n+1，cut[0]=false |
| res.bridge[id] | 原边 id 是否为桥 | 数组 m，与 g.edges 对齐 |

- **条件：** C++11；n≥0，端点必须在 1..n。n+1、边数、邻接列表长度和时间戳可由 int 表示，内存足够。不适用于有向图。
- **图构造：** 每条输入无向边只调用一次 add_edge，不再手动补反向调用。每条边拥有独立 id；重边与自环允许保留。
- **重边：** pe[u] 保存进入 u 的无向边编号，跳过的是该边，不是父节点本身。另一个连向父亲的重边仍是有效绕行。
- **判定：** 非根 p 存在树孩子 u 满足 low[u]≥dfn[p]，则 p 为割点；low[u]＞dfn[p] 时，pe[u] 为桥。DFS 根只看树孩子数是否大于 1，不按度数判断。
- **全图／多测：** 自动扫描全部未访问点，允许不连通图。每次调用重建状态；增加边后须重新求解，不是动态维护接口。返回数组独立保存。
- **删除语义：** 删除点同时删除其所有关联边；判断连通块数相对原图是否增加。孤立点、单个点不算割点；自环不算桥。
- **实现：** 显式栈模拟递归，长链不依赖系统栈深。next 推进每条邻接记录，孩子返回后才向父亲合并 low；不需要 SCC 的 pending／in；原因见[已访问邻点的祖先／后代对照](../../../notes/06-graph-and-tree/13-articulation-points-bridges.md#71-为什么-tarjan-scc-要检查-in而这里不需要)。
- **范围：** 不返回点双／边双分量，不维护动态删边，不自动排序桥的端点。输出顺序按题意自行处理；[P3388／P1656 使用复盘](../../../notes/06-graph-and-tree/13-articulation-points-bridges.md#9-典题与复盘)对照按点提取和按边编号还原端点。若要求无向边小端点在前并排序，先统一每对端点方向，再对端点对排序；库不改动原始 edges。

参考规则：[OI Wiki：割点和桥](https://oi-wiki.org/graph/cut/)。代码按仓库显式 DFS 风格自行实现。

[本章模板](../README.md) · [全部模板](../../README.md)
