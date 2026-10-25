# 第二章 序列与区间技巧：范式索引

区间、前后缀、值域与状态扫描。对应[本章知识笔记](../../notes/02-sequence-techniques/README.md)。

## 本章范式

- [二分答案：把优化与可行性判定分开](binary-search-on-answer.md)：补固定区间代价、可行长度与合法见证；ST 和单调队列可实现同一个判定。
- [前后缀分解：排除元素后的信息合并](prefix-suffix-exclusion.md)
- [后缀预处理：用未来摘要辅助当前决策](suffix-preprocessing.md)
- [排除不可行空间：约束扩张与区间收缩](feasible-interval-shrinking.md)
- [稀疏边界划段：按状态不变的区间计算](sparse-boundary-segments.md)
- [MEX：从定义域与频率出发](mex-analysis.md)
- [状态扫描：变化时结算与尾状态处理](state-scan-finalization.md)

## 跨章参考

- [等量统计：两类赋权与前缀差值分组](../08-strings/filter-key-dedup.md#等量统计把两类数量压成一个差值)：两类等量转成区间和为零，相同前缀值配对。

[返回全部范式](../README.md)
