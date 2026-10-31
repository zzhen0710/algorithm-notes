# 第一章 基础问题求解能力

从数据范围、朴素方案和可利用性质出发，建立问题分析与实现的基本方法。

> 数据范围 → 朴素方案 → 复杂度 → 可利用性质 → 合适算法

## 索引

| 条目 | 内容 |
| --- | --- |
| [1.1 复杂度分析](01-complexity-analysis.md) | 根据数据范围判断可行方法，分析实际总操作量 |
| [1.2 枚举](02-enumeration.md) | 自由变量、候选空间、约束降维与边界 |
| [1.3 模拟](03-simulation.md) | 最小状态、更新顺序、尾状态与规则建模、[行列与一维下标](03-simulation.md#24-一维下标与二维行列) |
| [1.4 STL 与竞赛常用 C++ 工具](04-stl/README.md) | 容器和标准算法的接口、复杂度、前提与选型 |
| [1.5 位运算](05-bit-operations.md) | 按位操作、位掩码、异或性质、lowbit、整数位宽与移位边界 |
| [1.6 本章总结与选型](06-summary-selection.md) | 枚举、模拟、STL、位运算的横向选择与 CF 467B 写法对比 |

基础排序原理参考 [ds-notes](https://github.com/zzhen0710/ds-notes)，普通排序使用 `sort`，需要稳定性时使用 `stable_sort`，不再单独安排冒泡、快排、堆排序等重复实现章节。排序建模与归并计数归入后续主题；需要自行实现的 KMP、并查集及图算法仍整理竞赛笔记与模板，详见 [知识总览](../README.md)。

## 笔记整理进度

勾选表示笔记已整理，学习掌握程度另行记录。

- [x] [1.1 复杂度分析](01-complexity-analysis.md)
- [x] [1.2 枚举](02-enumeration.md)
- [x] [1.3 模拟](03-simulation.md)
- [x] [1.4 STL 与竞赛常用 C++ 工具](04-stl/README.md)
- [x] [1.5 位运算](05-bit-operations.md)
- [x] [1.6 本章总结与选型](06-summary-selection.md)

## 本章范式

[查看本章范式索引](../../patterns/01-basic-problem-solving/README.md)，跨章节方法通过索引互相引用，完整正文只维护一处。

## 竞赛模板

[查看本章模板索引](../../templates/01-basic-problem-solving/README.md)，接口与代码在模板组内维护；具体知识正文保留相关模板直达。
