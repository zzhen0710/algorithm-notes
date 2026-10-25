# 第五章 动态规划：范式索引

通用建模、资源选择与最优性上界。对应[本章知识笔记](../../notes/05-dynamic-programming/README.md)。

## 本章范式

- [DP 三步走：找到状态、推导转移、写成代码](dp-modeling-workflow.md)：画状态、合并历史、完整转移与代码落实，含序列、双前缀、区间、[树上状态与依赖方向](dp-modeling-workflow.md#树上状态先看依赖父亲还是依赖孩子)、ST 倍增长度及线段树局部重算、摘要充分性及懒标记延迟分发的对照。
- [固定节点状态：让孩子子树独立求最优](subtree-boundary-state.md)：利用树的递归结构，保留父亲需要的条件，再独立合并孩子。
- [背包建模：使用次数、状态边界与优化目标](resource-limited-selection.md)
- [从最优性推出状态上界：超过太多，就删掉一步](optimality-state-bound.md)

## 跨章参考

- [DAG 上的 DP：先满足依赖，再汇总状态](../06-graph-and-tree/dag-dp.md)：图上的拓扑／逆拓扑顺序、状态传播、路径计数与适用边界。

[返回全部范式](../README.md)
