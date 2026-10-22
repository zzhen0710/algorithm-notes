# 第五章 动态规划：范式索引

资源选择、状态语义与最优性上界。对应[本章知识笔记](../../notes/05-dynamic-programming/README.md)。

## 本章范式

- [背包建模：使用次数、状态边界与优化目标](resource-limited-selection.md)
- [从最优性推出状态上界：超过太多，就删掉一步](optimality-state-bound.md)

## 跨章参考

- [DAG 上的 DP：先满足依赖，再汇总状态](../06-graph-and-tree/dag-dp.md)：含[序列中的隐式依赖](../06-graph-and-tree/dag-dp.md#从序列中识别隐式-dag)，对应 [5.4 序列 DP](../../notes/05-dynamic-programming/04-sequence-dp-lis.md)，其中[按长度保存最小末尾](../../notes/05-dynamic-programming/04-sequence-dp-lis.md#8-二分优化同长度只保留最小结尾)给出同问题的另一种状态与二分优化；逐位置长度、前后两侧合并及整体最优的区别见[合唱队形复盘](../../notes/05-dynamic-programming/04-sequence-dp-lis.md#92-洛谷-p1091合唱队形)。

- [两个前缀也能组成隐式 DAG](../06-graph-and-tree/dag-dp.md#两个前缀也能组成隐式-dag)：对应 [5.5 双序列 DP](../../notes/05-dynamic-programming/05-two-sequence-dp-lcs.md)，区分可用前缀与固定结尾，并说明状态维度如何影响复杂度；[最优子结构与贪心的区别](../06-graph-and-tree/dag-dp.md#取最优值不等于只走一条贪心路线)结合编辑距离与 LIS 解释同一状态为何能只留最优值。

[返回全部范式](../README.md)
