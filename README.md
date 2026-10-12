# Algorithm Notes

个人算法知识与解题复盘仓库。

记录算法知识、问题建模过程与有复用价值的解题经验。

## 仓库结构

### [Templates](templates/README.md)

已验证的竞赛算法模板。

用于沉淀便于现场复现、经过验证的竞赛实现。KMP、并查集、图算法等即使在相关仓库已有基础实现，也保留竞赛模板；验证状态如实记录。

### [Patterns](patterns/README.md)

常见解题范式与可复用的思维模型。

重点记录如何识别问题特征、选择方法并完成问题建模，而不仅仅是具体代码实现。

### [Problems](problems/README.md)

具有代表性和长期复习价值的题目与题解。

不记录全部刷题历史，只保留能够体现典型算法、解题思路、重要技巧或思维问题的题目。

### [Notes](notes/README.md)

按知识章节组织的系统算法笔记，以及学习过程中的经验总结与错误记录。

用于建立算法知识体系，记录概念、原理、复杂度、接口与边界；可复用实现、跨题范式和代表题分别由 Templates、Patterns、Problems 承担。

## 索引

> 根目录保留模块入口、代表性范式与专题总览；完整章节和详细选型请查看各目录索引。

### [Templates](templates/README.md)

- [C++ 竞赛基础模板](templates/cpp-contest-template/README.md)

### [Patterns](patterns/README.md)

- [枚举降维](patterns/enumeration-dimension-reduction.md)
- [前后缀分解：排除元素后的信息合并](patterns/prefix-suffix-exclusion.md)
- [MEX：从定义域与频率出发](patterns/mex-analysis.md)
- [博弈中的临界条件：从资源阈值到策略证明](patterns/critical-condition-in-game.md)
- [状态扫描：变化时结算与尾状态处理](patterns/state-scan-finalization.md)

### [Problems](problems/README.md)

- [CF 2271B — MEX Game](problems/CF_MEX_Game.md)

### [Notes](notes/README.md)

- [算法知识总览](notes/README.md)
- [STL 总览](notes/01-basic-problem-solving/04-stl/README.md)：[容器索引](notes/01-basic-problem-solving/04-stl/containers/README.md) · [算法索引](notes/01-basic-problem-solving/04-stl/algorithms/16-stl-algorithm-index.md)

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
