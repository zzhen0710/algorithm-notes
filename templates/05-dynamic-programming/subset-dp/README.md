# 小集合访问与有序选择

[知识笔记与执行图](../../../notes/05-dynamic-programming/08-subset-dp.md) · [实现](subset_dp.hpp)

保留两个**集合＋末尾**的具体模型：固定起点访问全集求最小值，以及任意起点恰好选 m 项求最大值。不是通用最短路算法，也不是所有状态压缩 DP 的统一接口。

## shortest_visit_path：固定起点访问全集

- **接口：** `subset_dp::shortest_visit_path(w,start=0)`，返回 ll 型最少花费；不恢复路线，不要求最后回到起点。
- **编号与容器：** 0-based，w 是 n×n 方阵，0≤n≤18；非空时 0≤start<n。
- **花费：** w[u][v] 是直接从 u 到 v 的花费；缺边填 `subset_dp::INF`，数值为 LLONG_MAX/4。有限边可为负，绝对值≤10¹²，所有候选和安全落在 ll 内且不会与 INF 混淆。
- **结果与边界：** 无法完成返回 INF；空表返回 0，忽略 start；n=1 不走边，返回 0。
- **初始化：** 只有起点位已置 1 的状态花费为 0，其余为 INF；可达值为 0 仍属合法状态。
- **副作用与多测：** 输入只读；每次调用创建局部 dp，不需要额外清理。调用者负责保证尺寸、起点和花费前提，接口不做越界修复。
- **复杂度：** 时间 O(n²·2ⁿ)，空间 O(n·2ⁿ)。n=18 时数值表约 36 MiB，另有二维 vector 与分配开销。
- **依赖：** C++11，标准头文件，无外部依赖。

不要把经过其他地点的最短路距离直接当作 w 中的一条普通边；否则实际访问集合可能与 mask 不一致。

## 使用示例

```cpp
#include "subset_dp.hpp"
#include <iostream>

int main() {
    std::vector<std::vector<long long>> w = {
        {0, 1, 4, 9},
        {9, 0, 2, 1},
        {9, 3, 0, 8},
        {9, 5, 7, 0}
    };

    auto ans = subset_dp::shortest_visit_path(w, 0);
    if (ans == subset_dp::INF) std::cout << "No path\n";
    else std::cout << ans << '\n'; // 8，对应 0→2→1→3
}
```

## 复现重点

1. mask 保存已访问集合，末尾 u 决定下一条边的起点；同 mask、同 u 才能直接比较历史花费。
2. 每次只能加入未选的 v，新的集合下标严格增大，因此 mask 正序传播。
3. 更新新格子的末尾是 v；先跳过不可达和缺边，再相加。
4. 全集一行取最小，因为没有固定终点；若题目改变访问数量、起点或目标，需重新定义初值、转移与答案位置。

## max_select_value：恰好选择 m 项，起点任意

- **接口：** `subset_dp::max_select_value(base,bonus,m)`；返回恰好选 m 个不同对象并排列后的最大收益。
- **收益定义：** 每个已选对象贡献一次 base；每对紧邻对象 u→v 贡献 bonus[u][v]。加分有方向，没有规则填 0，所有未选对象都允许接在末尾。
- **输入：** 0-based，n=base.size()，0≤n≤18、0≤m≤n，bonus 为 n×n 方阵；base 与 bonus 可为负，绝对值≤10¹²。候选总和不超过 35×10¹² 的绝对值，ll 与哨兵范围安全。
- **初始化与结果：** 全部单点 dp[1<<u][u]=base[u]，其他 −INF；选够 m 个即收答案、不再扩展。m=0（包含空输入）返回 0，合法输入始终可以选够，不表示存在额外禁配约束。
- **副作用与多测：** 两个输入只读，局部 dp 每次重建；尺寸与 m 由调用处保证。
- **复杂度：** 最坏时间 O(n²·2ⁿ)，空间 O(n·2ⁿ)；最多只向 m 个对象的集合传播。n=18 的数值表约 36 MiB，另有 vector 开销。
- **依赖：** C++11；计数使用 GCC／Clang 的 `__builtin_popcount`。

```cpp
// 接在上面示例的 main 中；3 道选 2 道，0→1 额外加 10。
std::vector<long long> base = {1, 2, 3};
std::vector<std::vector<long long>> bonus(3, std::vector<long long>(3));
bonus[0][1] = 10;
std::cout << subset_dp::max_select_value(base, bonus, 2) << '\n'; // 13
```

**用途速记：** 输入单项收益与相邻加分，求固定数量的不重复有序选择最大收益。所有起点一起初始化，按 mask 递增传播，再按 1 的数量收集终态；不能用一段整数范围代替集合大小筛选。

第一题的 double 距离版本留在[知识笔记](../../../notes/05-dynamic-programming/08-subset-dp.md#81-p1433把原点也当作一个地点)，不复制一份只改类型的模板。两题原实现与改进版已本地核验，平台结果未单独提供。

[本章模板](../README.md) · [全部模板](../../README.md)
