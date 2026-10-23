# Templates

存放便于现场复现、具有复用价值的竞赛算法实现，按实际学习与验证结果逐项整理。

## 索引

- [C++ 竞赛基础模板](cpp-contest-template/README.md)
- [离散化：建表与编号查找](discretization/README.md)
- [整数二分：可行边界与独立判定](binary-search/README.md)
- [归并计数：严格逆序对](merge-counting/README.md)
- [区间选择：最多互不冲突区间](interval-scheduling/README.md)
- [全图二分图染色与判定](bipartite-coloring/README.md)
- [并查集：路径压缩与按大小合并](dsu/README.md)
- [树状数组：增量与前缀和](fenwick/README.md)
- [Kruskal：最小生成树与目标块数](kruskal/README.md)
- [gcd/lcm：递归与数组聚合](gcd-lcm/README.md)
- [快速幂：整数模幂](binary-exponentiation/README.md)
- [素数：判定、两种筛法与质因数分解](primes/README.md)
- [精确组合数：单次乘除递推](combinations/README.md)
- [KMP：前缀函数与全部匹配位置](kmp/README.md)
- [字符串双哈希：子串指纹与去重键](string-hash/README.md)

- [Dijkstra：非负权最短路与路径还原](dijkstra/README.md)
- [拓扑排序：Kahn 顺序与有向环判定](topological-sort/README.md)
- [二分图最大匹配：DFS 增广](bipartite-matching/README.md)
- [LIS：二分长度与逐位置长度](lis/README.md)
- [单调栈：右侧第一个严格更大位置](monotonic-stack/README.md)
- [单调队列：定长窗口最大值位置](monotonic-queue/README.md)
- [区间合并：合并覆盖与并集长度](interval-merging/README.md)

- [ST 表：静态闭区间最大值](sparse-table/README.md)

## 收录范围

普通排序和基础容器优先使用 STL，不重复实现整套教材内容。KMP、并查集、最短路、最小生成树、拓扑排序等即使在 [ds-notes](https://github.com/zzhen0710/ds-notes) 已有实现，也可整理为本仓库的竞赛模板。

先完成 B 组及其包含的 C 组内容，再按需补充 A 组专项。A 组条目在标题与索引前显示 `*`，正文注明“扩展内容”；星号不表示难度。知识范围见 [Notes 总览](../notes/README.md)。

## 模板规范

- 一项算法或一组紧密相关操作使用一个目录，附简短 README 与实现文件；有内容时再创建。
- 优先使用短函数或必要的结构体，避免照搬教学演示类、打印工具和多文件工程封装。
- 明确接口、下标起点、区间端点、输入前提、返回值、复杂度与关键状态含义，代码添加必要注释。
- 记录初始化、多组测试清理、整数溢出、空输入或不可达等边界；按算法选择适用的验证案例。
- 每项优先维护一种主写法；变体只说明差异，避免多份实现长期不一致。
- 成熟竞赛库作为比较与参考来源；若自行实现更简洁易用、契约更合适，就采用自行实现，不强制沿用外部代码。采用外部代码时记录原文件、固定版本、许可与适配差异，再验证本地版本，不把上游测试直接当作适配后的验证。
- 模板面向比赛时的简洁易用，笔记代码面向理解过程，允许两者不同；说明关键差异并分别核验，不强行维持同一份写法。
- 如实记录编译、样例、边界或对拍结果；代码验证和个人独立复现情况分开记录，不把模板整理完成视为已经掌握。
- DP 与贪心按具体模型保留实现示例，不建立万能模板。
- 典题核验后可从稳定实现提炼模板，记录来源笔记或题解并互链；个人解法的历史、错误与比较留在典题复盘中。练习前可提供通用算法模板，但不借模板示例提前泄露待做题的关键解法。

KMP 统一采用 0 下标的前缀函数写法：`pi[i]` 表示截至 `i` 的最长相等真前后缀长度；明确空模式串约定、重叠匹配、失配及完整匹配后的回退。其他 `next` 定义通过对应关系说明。
