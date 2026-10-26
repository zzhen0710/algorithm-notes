# 第八章 字符串

承接 [string 基本操作](../01-basic-problem-solving/04-stl/containers/03-string.md)，整理竞赛中需要自行实现的匹配、前后缀与回文模型。已有 DS 基础复用，不重复基础容器实现。

## 索引

- [8.1 KMP 与前缀函数](01-kmp.md)：0-based pi、失配与重叠匹配、内部出现上界与 border 候选链。
- [8.2 字符串哈希与碰撞边界](02-string-hash.md)：前缀指纹、区间换算、碰撞边界；筛合法候选后排序去重的两题复盘。

- [8.3 Manacher：用镜像信息加速回文扩展](03-manacher.md)：用同一例子的连续执行图讲清半径与位置换算；两题复盘串起最长回文与前后缀结构缩减。

- 跨章参考：[9.8 Trie：共享前缀与字符串计数](../09-data-structures/08-trie.md)，正文与模板归第九章，不重复维护。

## 笔记整理进度

勾选只表示知识正文已整理，不代表配题完成或已经熟练；本章尚未整理完毕。

- [x] [8.1 KMP 与前缀函数](01-kmp.md)：正文与 P3375、CF 126B 原实现复盘完成（本地核验通过，平台结果未单独记录）；补内部前缀出现、mx 上界和 border 回退图解。
- [x] [8.2 字符串哈希与碰撞边界](02-string-hash.md)：知识正文、P3370 与 CF 271D 原实现复盘完成（本地核验，平台结果未单独记录）；已提炼[比较键范式](../../patterns/08-strings/filter-key-dedup.md)与[双哈希模板](../../templates/08-strings/string-hash/README.md)。
- [x] [8.3 Manacher](03-manacher.md)：正文、逐轮图解、P3805／CF1326D2 原实现及通用模板已本地核验并复盘，平台结果未单独提供；提炼[答案结构缩减](../../patterns/04-greedy-and-construction/answer-structure-reduction.md)，熟练度另行核验。
- [ ] 前后缀与周期的进一步应用及其余字符串内容。
- [ ] 全章总结与场景选型，主题收尾时完善。

## 学习衔接

基础链先完成 KMP 的状态含义、回退与典题，再认识哈希的适用条件和碰撞风险。8.2 两题复盘已整理，单调结构与区间维护已按实际记录推进。多字符串前缀检索衔接 [9.8 Trie](../09-data-structures/08-trie.md)；熟练度通过手推与独立重写另行核验。高精度基础单元复盘后，接入 [8.3 Manacher](03-manacher.md)，建立回文中心、镜像复用与线性边界推进模型；其余内容继续按前置关系补充，不预建空文章。

本章主 KMP 写法统一使用 0-based 前缀函数 pi；其他 next 约定只作定义对照，避免多个版本混用。通用框架见 [C++ 竞赛基础模板](../../templates/01-basic-problem-solving/cpp-contest-template/README.md)。

知识范围与典题复盘规范见 [Notes 总览](../README.md)。

## 本章范式

[查看本章范式索引](../../patterns/08-strings/README.md)，跨章节方法通过索引互相引用，完整正文只维护一处。

## 竞赛模板

[查看本章模板索引](../../templates/08-strings/README.md)，接口与代码在模板组内维护；具体知识正文保留相关模板直达。
