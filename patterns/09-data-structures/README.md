# 第九章 竞赛数据结构：范式索引

对应[本章知识笔记](../../notes/09-data-structures/README.md)。记录摘要选择、动态维护与操作组合的识别方法，具体树结构与题解留在 Notes。

## 本章范式

- [统一变换，按顺序复合](composable-transformations.md)：多种操作统一表示、先后顺序、复合封闭、摘要更新与翻转抵消。

## 跨章参考

- [固定依赖与局部维护](../05-dynamic-programming/dp-modeling-workflow.md#固定划分的区间只重算受影响的祖先)：节点摘要是否足够、局部重算与延迟分发。
- [区间代价与二分长度](../02-sequence-techniques/binary-search-on-answer.md#固定区间代价再二分可行长度)：查询工具与可行性判定分层。
- [幂长状态与计算顺序](../05-dynamic-programming/dp-modeling-workflow.md#只存幂长区间按倍增长度满足依赖)：ST 表与依赖关系。

[返回全部范式](../README.md)
