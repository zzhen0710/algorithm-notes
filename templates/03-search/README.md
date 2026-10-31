# 第三章 搜索：模板索引

[对应知识笔记](../../notes/03-search/README.md)

## 本章模板

- [小规模 0/1 选择：分支限界](branch-and-bound/README.md)：正重量、非负价值，容量为上限，只返回最优值；后缀价值作为上界。

- [小规模一维装箱](bin-packing/README.md)：所有物品必须放入同容量箱子，求最少箱数；维护有效载重前缀，允许任意件同箱。

- [双向 BFS](bidirectional-bfs/README.md)：指定起终点、等代价；由调用方生成正向后继与反向前驱，整层扩展并可限制总步数。

- [迭代加深 IDDFS](iterative-deepening/README.md)：给定目标与合法后继，限制最大深度，返回一条最少步数路径；使用当前路径标记。

- [启发式搜索 A*／IDA*](heuristic-search/README.md)：单位步长、合法非负下界；同一组回调可替换调用，步数上限可省略；前者用 pq，后者用 f 门槛 DFS。

不建立万能回溯或万能剪枝函数。具体状态与界需要据题重新设计；记忆化与递推的选型见[关联范式](../../patterns/05-dynamic-programming/memoization-vs-tabulation.md)。

[返回全部模板](../README.md)
