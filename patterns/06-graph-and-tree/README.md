# 第六章 图与树：范式索引

无环依赖、状态方向与结果汇总。对应[本章知识笔记](../../notes/06-graph-and-tree/README.md)。

## 本章范式

- [DAG 上的 DP：先满足依赖，再汇总状态](dag-dp.md)：图上的依赖、拓扑传播、状态方向、路径统计；补充[树上的父子依赖与根路径前缀](dag-dp.md#树上的根路径前缀父亲先于孩子)。

## 跨章参考

- [翻转历史与异或差分](../09-data-structures/composable-transformations.md#翻转历史的奇偶性也能用异或差分维护)：DFS序负责子树映射，奇偶状态负责压缩翻转，最终输出时统一还原。

- [DP 三步走](../05-dynamic-programming/dp-modeling-workflow.md)：通用状态设计、最优子结构及序列／前缀／区间的依赖对照。

[返回全部范式](../README.md)
