# 第三章 搜索：范式索引

围绕一层决策、完整状态与等价分支组织搜索。对应[本章知识笔记](../../notes/03-search/README.md)。

## 本章范式

- [逐个对象分配容器：先定一层工作，再记录容器状态](object-to-container-search.md)：全部对象都要安置时，枚举去向、维护容器载重、统一新容器编号、区分顺序优化与剪枝；结合数的划分对照唯一表示。

## 跨章参考

- [搜索末端转为方程求解](../01-basic-problem-solving/enumeration-dimension-reduction.md#搜索末端变量少了可以换成方程)：前缀固定后重新检查剩余等式；结合 [P1763](../../notes/03-search/06-iterative-deepening.md#81-p1763埃及分数限项数再化简搜索末端)区分限深、范围放宽与韦达求根。

- [记忆化搜索与数组递推](../05-dynamic-programming/memoization-vs-tabulation.md)：完整状态、计算顺序与选型，尤其是[剪枝返回与精确缓存](../05-dynamic-programming/memoization-vs-tabulation.md#剪枝停止探索不等于已经算完子问题)。
- [DAG 上的 DP](../06-graph-and-tree/dag-dp.md)：依赖方向、状态结果与转移组织。

## 知识衔接

- [3.5 双向 BFS](../../notes/03-search/05-bidirectional-bfs.md)：等代价、目标已知且可生成前驱时，比较从两端搜索；保留两侧完整分层，不能把逐节点交替的第一次碰头直接当最短路。标准算法在 Notes 维护，不重复新建范式；两题进一步对照[共用骨架与题目转移](../../notes/03-search/05-bidirectional-bfs.md#93-两题共用骨架题目决定一步怎么走)，坐标和工厂语法分别链接基础笔记。

- [3.6 迭代加深](../../notes/03-search/06-iterative-deepening.md)：逐次限制深度，比较少存宽层状态与重复浅层搜索；[剩余步数与路径标记](../../notes/03-search/06-iterative-deepening.md#不能把首次到达永久标记成以后都不用再搜)不能机械套用 BFS 永久判重。

- [3.7 启发式搜索](../../notes/03-search/07-heuristic-search.md)：[放松限制设计下界](../../notes/03-search/07-heuristic-search.md#3-启发函数设计)，分别交给 pq 与限深 DFS 管理；[本质与动态对比](../../notes/03-search/07-heuristic-search.md#5-本质与横向对比)集中展示，两套模板复用相同的 Step／Goal／Heuristic。

[返回全部范式](../README.md)
