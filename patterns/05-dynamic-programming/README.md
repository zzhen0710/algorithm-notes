# 第五章 动态规划：范式索引

通用建模、资源选择与最优性上界。对应[本章知识笔记](../../notes/05-dynamic-programming/README.md)。

## 本章范式

- [记忆化搜索与数组递推：同一状态，怎样安排计算](memoization-vs-tabulation.md)：参数与下标、状态循环与转移循环、前后缀计数方向及场景选型；补[剪枝返回与精确缓存的区别](memoization-vs-tabulation.md#剪枝停止探索不等于已经算完子问题)。

- [DP 三步走：找到状态、推导转移、写成代码](dp-modeling-workflow.md)：[识别到压缩的五项检查](dp-modeling-workflow.md#0-从识别到压缩五项检查)、画状态、合并历史、完整转移与代码落实，含序列、双前缀、区间、[集合与末尾状态](dp-modeling-workflow.md#集合状态保留已选对象与必要的末尾)及起点、可达性与终态检查、[树上状态与依赖方向](dp-modeling-workflow.md#树上状态先看依赖父亲还是依赖孩子)、[Floyd 中转阶段](dp-modeling-workflow.md#按可用中转点分阶段floyd)、[数位条件与上界](dp-modeling-workflow.md#数位状态保留条件与上界限制)、ST 倍增长度及线段树局部重算、摘要充分性及懒标记延迟分发的对照。
- [固定节点状态：让孩子子树独立求最优](subtree-boundary-state.md)：利用树的递归结构，保留父亲需要的条件，再独立合并孩子。
- [背包建模：使用次数、状态边界与优化目标](resource-limited-selection.md)
- [从最优性推出状态上界：超过太多，就删掉一步](optimality-state-bound.md)

## 跨章参考

- [逐个对象分配容器](../03-search/object-to-container-search.md)：对照“选不选”与“放到哪里”；共享载重数组也属于状态，不能只按 DFS 的两个形参建立缓存。

- [DAG 上的 DP：先满足依赖，再汇总状态](../06-graph-and-tree/dag-dp.md)：图上的拓扑／逆拓扑顺序、状态传播、路径计数与适用边界。

- [从递推式构造矩阵](../07-math/recurrence-to-matrix.md)：把下一步需要的历史一起保存，再逐格写更新；固定线性转移且下标很大时，用矩阵快速幂跳过逐轮计算。

[返回全部范式](../README.md)
