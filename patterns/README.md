# Patterns：按章节查找范式

记录跨题识别与建模方法。**章节编号和目录名与 Notes 对齐，先进入章节索引，再查阅完整条目。**

## 章节索引

| 对应章节 | 范式入口 | 主要内容 |
| --- | --- | --- |
| [01 基础问题求解](../notes/01-basic-problem-solving/README.md) | [本章索引](01-basic-problem-solving/README.md) | 枚举降维、少量共享变量 |
| [02 序列与区间技巧](../notes/02-sequence-techniques/README.md) | [本章索引](02-sequence-techniques/README.md) | 区间、前后缀、二分、MEX 与扫描结算 |
| [03 搜索](../notes/03-search/README.md) | 跨章参考：[DAG DP](06-graph-and-tree/dag-dp.md) | 暂无独立篇章，记忆化与依赖状态见关联范式 |
| [04 贪心与构造](../notes/04-greedy-and-construction/README.md) | [本章索引](04-greedy-and-construction/README.md) | 基值与剩余量、分阶段解耦 |
| [05 动态规划](../notes/05-dynamic-programming/README.md) | [本章索引](05-dynamic-programming/README.md) | DP 建模三步、背包识别、状态上界、子树条件状态，并链接 DAG DP |
| [06 图与树](../notes/06-graph-and-tree/README.md) | [本章索引](06-graph-and-tree/README.md) | 无环依赖、状态方向、路径统计与父子递推 |
| [07 数学](../notes/07-math/README.md) | [本章索引](07-math/README.md) | 博弈阈值与策略证明 |
| [08 字符串](../notes/08-strings/README.md) | [本章索引](08-strings/README.md) | 筛选候选、比较键与指纹去重 |
| [09 竞赛数据结构](../notes/09-data-structures/README.md) | [本章索引](09-data-structures/README.md) | 统一变换、操作复合与翻转奇偶；跨章引用局部维护、倍增状态与区间判定 |

## 归档与跨章引用

- 每篇范式只选一个主要归属；跨章节使用时，在相关章节索引引用，不复制正文。
- 尚无独立范式的章节不创建空目录；首篇正式内容加入时再建立章节 README。
- 本章目录内保持单层文件，文件名继续表达方法，不按来源题目命名。
- 新建或实质修订知识笔记、完成典题复盘时，检查已有范式；发生新增或补充时同步章索引、跨章引用及 Notes 双向链接，必要时更新本表，不为每篇笔记强造新范式。

## 提炼原则

从已核验的典题复盘中发现可跨题复用的方法时，先检索本目录；已有相同范式则补充例证或边界，避免重复命名。

值得独立成篇的方法应能说明：题目中的触发线索、可复用模型、成立条件、从朴素做法到改进的推导、复杂度变化，以及不适用情形或反例。仅把题目换个标题，或重述一个标准算法，不足以构成新范式。

不机械要求先积累固定题数；一题也可提炼，但需要解释迁移到其他问题的依据。只有单题观察、尚不确定能否泛化时，先留在原题复盘中。

范式篇记录通用模型与必要伪代码，具体的“我的做法／推荐做法”留在原笔记或独立题解，通过双向链接关联。完整条目维护于所属章节的 README；根 README 只保留少量重点直达，新增范式不自动加入。
