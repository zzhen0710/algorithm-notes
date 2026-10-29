# 第六章 图与树：范式索引

无环依赖、状态方向、逆序离线与结果汇总。对应[本章知识笔记](../../notes/06-graph-and-tree/README.md)。

## 本章范式

- [DAG 上的 DP：先满足依赖，再汇总状态](dag-dp.md)：图上的依赖、拓扑传播、状态方向、路径统计；补充[树上的父子依赖与根路径前缀](dag-dp.md#树上的根路径前缀父亲先于孩子)。

- [逆序离线：把难维护的删除变成恢复](reverse-offline.md)：操作方向选择、初始状态、答案时机与 Floyd 逐阶段恢复。

## 跨章参考

- [割点与桥：先明确删点还是删边](../../notes/06-graph-and-tree/13-articulation-points-bridges.md#5-非根割点只能回到-u-本身也救不了删点)：通过能否绕开对象推导 low 的比较；补[点／边结果提取与端点映射](../../notes/06-graph-and-tree/13-articulation-points-bridges.md#93-两题共同留下什么)。与 SCC 的目标和状态边界分开，暂不独立重复范式。

- [强连通分量：缩点后衔接 DAG](../../notes/06-graph-and-tree/12-strongly-connected-components.md#5-缩点原点编号映射为分量编号)：先解决有向环内互相可达，再分析组间依赖；组内状态能否聚合须另证；[最少起点覆盖](../../notes/06-graph-and-tree/12-strongly-connected-components.md#92-p2002最少选几个起点才能让消息覆盖全图)补原图入度反例与源分量证明。

- [差分约束：从限制反推图](../../notes/06-graph-and-tree/11-difference-constraints.md#2-为什么是-j-到-i看谁为谁提供上界)：先统一形式，再从松弛公式确认方向；可行性与优化目标分开。当前保留知识笔记入口，不另建重复范式。

- [按允许边数分阶段：Bellman–Ford](../05-dynamic-programming/dp-modeling-workflow.md#按允许边数分阶段bellmanford)：原地松弛与严格阶段的差别；[负环范围及队列传播](../../notes/06-graph-and-tree/10-negative-shortest-path.md#4-检测范围从一个点出发还是检查整张图)在知识笔记维护。

- [按可用中转点分阶段：Floyd](../05-dynamic-programming/dp-modeling-workflow.md#按可用中转点分阶段floyd)：原图可有环，阶段依赖仍按可用中转范围递增。

- [等量统计：赋权后求子树和](../08-strings/filter-key-dedup.md#子树先映射成区间再求权值和)：两类节点分别记为 +1、−1，结合 DFS 序与前缀和判断等量。

- [固定节点状态与子树独立合并](../05-dynamic-programming/subtree-boundary-state.md)：用当前节点的状态表达父子约束；与只满足计算顺序分开考虑。

- [树形 DP 的依赖方向](../05-dynamic-programming/dp-modeling-workflow.md#树上状态先看依赖父亲还是依赖孩子)：父亲先算与孩子先算、顺序与组合规则的分工。

- [翻转历史与异或差分](../09-data-structures/composable-transformations.md#翻转历史的奇偶性也能用异或差分维护)：DFS序负责子树映射，奇偶状态负责压缩翻转，最终输出时统一还原。

- [DP 三步走](../05-dynamic-programming/dp-modeling-workflow.md)：通用状态设计、最优子结构与依赖对照；补[完整建模到空间压缩的检查流程](../05-dynamic-programming/dp-modeling-workflow.md#0-从识别到压缩五项检查)。

[返回全部范式](../README.md)
