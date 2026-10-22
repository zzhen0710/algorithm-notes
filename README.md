# Algorithm Notes

个人算法知识与解题复盘仓库。

记录算法知识、问题建模过程与有复用价值的解题经验。

## 导航

> **根目录提供模块入口与少量重点直达；完整条目请查看各模块索引。**

| 模块 | 完整索引 | 内容 | 重点直达 |
| --- | --- | --- | --- |
| Notes | [算法知识总览](notes/README.md) | 系统知识、配套典题与双解复盘 | [STL 总览](notes/01-basic-problem-solving/04-stl/README.md) |
| Patterns | [范式索引](patterns/README.md) | 跨题识别与问题建模 | [枚举降维](patterns/01-basic-problem-solving/enumeration-dimension-reduction.md) · [前后缀分解](patterns/02-sequence-techniques/prefix-suffix-exclusion.md) · [稀疏边界划段](patterns/02-sequence-techniques/sparse-boundary-segments.md) |
| Templates | [模板索引](templates/README.md) | 带接口与边界约定的可复用实现 | [C++ 竞赛基础框架](templates/cpp-contest-template/README.md) |
| Problems | [好题复盘索引](problems/README.md) | 独立刷题中精选的题目与复盘 | [CF 2271B — MEX Game](<problems/CF 2271B_MEX_Game.md>) |

配套典题的原实现、推荐实现与改进分析完整保留在所属知识笔记，独立刷题精选才归入 Problems。

典题按“给题 → 独立尝试 → 核验 → 我的做法与推荐做法对比 → 提炼迁移方法”推进，具体规范见 [Notes 总览](notes/README.md)。

## 收录原则

这个仓库不会记录做过的每一道题。

当一道题或一次学习产生以下至少一种价值时，才值得沉淀：

- 学习了新的算法或数据结构
- 总结出了可复用的解题范式
- 遇到了具有代表性的题目
- 发现了重要的错误、边界条件或思维盲点
- 得到了更简洁、更通用的实现方式

目标不是积累代码数量，而是在持续训练中逐渐建立一个精炼、可复用的个人算法知识库。

## 相关仓库与知识分工

[ds-notes](https://github.com/zzhen0710/ds-notes) 收录数据结构基础，以及排序、查找、图的基础算法和 KMP。本仓库承接这些基础，整理竞赛所需的必要原理、规范模板、问题建模与应用边界。普通排序及基础容器优先使用 STL，不重复编写教材式实现。

知识目录保持九个主题，详见 [Notes 总览](notes/README.md)。先整理 B 组及其包含的 C 组内容，全部整理后再按需补充 A 组专项；A 组条目的标题与索引前标记 `*`，星号表示扩展范围，不表示难度。
