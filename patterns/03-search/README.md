# 第三章 搜索：范式索引

围绕一层决策、完整状态与等价分支组织搜索。对应[本章知识笔记](../../notes/03-search/README.md)。

## 本章范式

- [逐个对象分配容器：先定一层工作，再记录容器状态](object-to-container-search.md)：全部对象都要安置时，枚举去向、维护容器载重、统一新容器编号、区分顺序优化与剪枝；结合数的划分对照唯一表示。

## 跨章参考

- [记忆化搜索与数组递推](../05-dynamic-programming/memoization-vs-tabulation.md)：完整状态、计算顺序与选型，尤其是[剪枝返回与精确缓存](../05-dynamic-programming/memoization-vs-tabulation.md#剪枝停止探索不等于已经算完子问题)。
- [DAG 上的 DP](../06-graph-and-tree/dag-dp.md)：依赖方向、状态结果与转移组织。

[返回全部范式](../README.md)
