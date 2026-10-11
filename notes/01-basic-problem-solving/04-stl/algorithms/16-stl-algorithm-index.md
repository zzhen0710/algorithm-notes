# 16 · STL Algorithm 总结索引

本篇汇总本目录的标准算法笔记，按“知识索引 → 竞赛选型速查 → 可复用思想 → 学习说明”组织，作为整章的复习入口。

重点掌握：

- 算法解决什么需求，要求什么输入性质。
- 参数、返回值和有效结果区间。
- 复杂度、迭代器要求及常见陷阱。
- 如何组合已有工具完成问题建模与实现。

本章以 **C++11** 为兼容基准。`accumulate / iota` 属于 `<numeric>`，通用 `swap` 属于 `<utility>`，仍放在本章统一复习。

## 一、知识索引

### 1. 排序、边界与选择

| 笔记 | 核心内容 |
| --- | --- |
| [01 · sort / stable_sort](01-sort.md) | 比较器、严格弱序、稳定性与多关键字排序 |
| [02 · lower_bound / upper_bound](02-binary-search.md) | 边界位置、`binary_search / equal_range`、成员接口与通用算法的区别 |
| [12 · partial_sort / nth_element](12-partial-sort-nth-element.md) | 前 k 小、第 k 小、中位数与部分排序 |

### 2. 筛选、查找与聚合

| 笔记 | 核心内容 |
| --- | --- |
| [03 · unique / erase-remove](03-unique-remove.md) | 相邻去重、逻辑移除、容器删除与离散化 |
| [04 · accumulate](04-accumulate.md) | 区间聚合、初值类型与自定义累计操作 |
| [05 · min_element / max_element](05-min-max.md) | 极值位置、`minmax_element` 与比较规则 |
| [09 · find / count / mismatch](09-find-count-mismatch.md) | 线性查找、条件统计、序列比较与首个差异 |

### 3. 重排、排列与有序合并

| 笔记 | 核心内容 |
| --- | --- |
| [06 · reverse / rotate](06-reverse-rotate.md) | 反转、循环移位与区间结构变换 |
| [07 · next_permutation / prev_permutation](07-next-permutation.md) | 字典序、排列枚举、重复元素与 DFS 对比 |
| [10 · partition / stable_partition](10-partition-stable_partition.md) | 按条件分组、返回分界与保序需求 |
| [11 · merge / 集合操作](11-merge-set-operations.md) | 有序合并、相邻区间归并、并集、交集与差集 |

### 4. 初始化、映射与函数工具

| 笔记 | 核心内容 |
| --- | --- |
| [08 · fill / iota / generate](08-fill-iota-generate.md) | 批量初始化、自定义生成与排序下标技巧 |
| [13 · copy / copy_if / transform](13-copy-copy_if%20-transform.md) | 复制、保序筛选、一元与二元映射、输出空间 |
| [14 · swap / iter_swap](14-swap-iter_swap.md) | 交换元素、交换迭代器变量与复杂度区别 |
| [15 · Lambda 与比较器](15-lambda-comparators.md) | 捕获方式、函数角色、严格弱序与稳定性 |

## 二、竞赛选型速查

| 需求 | 优先考虑 | 关键条件或提醒 |
| --- | --- | --- |
| 所有元素排序 | `sort` | 随机访问迭代器；比较器满足严格弱序 |
| 排序并保留等价元素的原顺序 | `stable_sort` | 等价由比较器定义 |
| 升序区间中第一个不小于 x 的位置 | `lower_bound` | 返回迭代器，可能为 `end()` |
| 升序区间中第一个大于 x 的位置 | `upper_bound` | 与当前排序规则一致 |
| 判断有序区间中是否存在 x | `binary_search` | 返回 `bool` |
| 查找 x 的整个等价区间 | `equal_range` | 返回两个边界迭代器 |
| 无序区间中找元素或符合条件的位置 | `find / find_if` | 线性查找；失败返回 `end()` |
| 数组全局去重 | `sort + unique + erase` | 排序会改变原顺序 |
| 保序删除符合条件的元素 | `remove_if + erase` | 算法逻辑移除后，再由容器删除 |
| 求和或自定义聚合 | `accumulate` | 初值决定累计类型；大整数和常用 `0LL` |
| 找极值或其位置 | `min_element / max_element` | 返回迭代器；空区间不可解引用 |
| 统计数量 | `count / count_if` | 不要求有序 |
| 找两个序列的首个差异 | `mismatch` | C++11 三迭代器版本需自行保证第二序列长度 |
| 反转或循环移动区间 | `reverse / rotate` | 双向或前向迭代器要求因算法而异 |
| 枚举所有不同排列 | `sort + do-while + next_permutation` | 从最小字典序开始；数量可能为阶乘级 |
| 将满足条件的元素放到前面 | `partition` | 返回分界；组内顺序不保证 |
| 按条件分组并保持各组相对顺序 | `stable_partition` | 保序需求可能增加时间或内存开销 |
| 合并两个有序区间 | `merge` | 同一排序规则；输出不能覆盖输入 |
| 合并同一序列中相邻的两个有序部分 | `inplace_merge` | 两部分分别有序；可能申请辅助内存 |
| 有序序列的并、交、差 | `set_union / set_intersection / set_difference` | 允许重复元素，结果按相应重数规则产生 |
| 前 k 小且有序 | `partial_sort` | 只保证前 k 项有序 |
| 第 k 小或无需有序的前 k 小 | `nth_element` | 第 k 小对应下标 k−1；左右内部不排序 |
| 填同一个值、连续编号、自定义生成 | `fill / iota / generate` | 通过普通迭代器写入时，目标元素必须存在 |
| 复制或保序筛选到另一个容器 | `copy / copy_if` | 输出数量未知时可用 `back_inserter` |
| 每个元素转换，或两组数据逐项合成 | `transform` | 二元版本由第一个输入区间决定处理长度 |
| 交换两个值或两个位置的元素 | `swap / iter_swap` | `swap(it1, it2)` 交换的是迭代器变量 |

### 常用复杂度速记

设区间长度为 $n$，第二个区间长度为 $m$，选取数量为 $k$。

| 操作 | 复杂度或主要工作量 |
| --- | --- |
| `sort` | $O(n\log n)$ 次比较 |
| 有序 `vector` 上的二分接口 | $O(\log n)$ 次比较和迭代器操作 |
| 线性查找、去重、移除、聚合、极值、复制与转换 | 通常 $O(n)$；计入用户函数和元素操作成本 |
| `reverse / rotate` | $O(n)$ |
| 单次 `next_permutation / prev_permutation` | 最坏 $O(n)$；全排列总成本还要乘排列数量 |
| `partition` | $n$ 次谓词判断，线性数量的交换 |
| `merge` 与两区间集合算法 | $O(n+m)$ |
| `partial_sort` | 常记 $O(n\log k)$，适用于 $2\le k\le n$；$k=1$ 时为 $O(n)$ |
| `nth_element` | 平均 $O(n)$，不能据此写成保证最坏线性 |

`stable_sort / stable_partition / inplace_merge` 的复杂度还与辅助内存是否可用有关；不要把它们与不保序版本或独立输出版本简单视为同一开销。二分用于非随机访问迭代器时，比较次数虽为对数级，迭代器移动仍可能为线性级；`set / map` 的查边界优先使用成员函数。

## 三、值得复用的解题思想

**1. 先判断答案需要多少顺序信息**

完整有序、前 k 项有序、第 k 个位置正确，是不同需求。只找一个极值就线性扫描，只找第 k 小就考虑选择算法，避免无必要的完整排序。

**2. 先建立性质，再使用依赖该性质的工具**

排序可以建立单调性，让边界查询、相邻去重和有序合并变得简单。算法前提必须先成立，不能只因为接口名称合适就直接调用。

**3. 用区间和返回位置描述有效结果**

`unique / remove_if` 返回新的逻辑终点，`partition` 返回分组边界，二分返回查询位置。先确认这个位置的含义，再决定遍历、删除或解引用。

**4. 分离原数据与处理顺序**

利用 `iota` 生成编号，再让比较器读取原数据、排序编号，可以同时保留原始数据与排名关系。

**5. 将简单循环表达为复制、筛选、映射或聚合**

原样搬运使用 `copy`，按条件另存使用 `copy_if`，逐项生成新值使用 `transform`，累积结果使用 `accumulate`。复杂状态转移、提前退出等需求仍应选择易读的显式循环。

## 四、学习说明与易错点

- **区间采用 `[first, last)`。** `last / end()` 通常不可解引用；空区间要单独考虑。
- **输出空间由调用者负责。** 普通输出迭代器覆盖已有元素，`back_inserter` 才会追加；`reserve` 不增加 `size()`。
- **算法与容器各有职责。** `unique / remove_if` 不改变容器大小；`erase` 才执行实际删除。
- **比较规则必须一致。** 排序、二分和有序集合操作应使用相容的比较关系；严格比较不能写成 `<=`。
- **筛选方向看接口语义。** `copy_if` 的 true 表示复制，`remove_if` 的 true 表示移除。
- **原地不代表零辅助内存。** “结果写回原区间”与“辅助空间为常数”是两个不同问题。
- **二元输入自行检查长度。** C++11 的二元 `transform`、三迭代器 `mismatch / equal` 不会自动按较短序列结束。
- **标准保证与常见实现分开记。** 平均复杂度、比较次数、交换次数、辅助空间不能混成一个结论。

本轮规划的 **01–16 节笔记整理至此**。复习时先通过本篇选型，再进入对应章节核对 API 和边界；掌握程度仍以独立实现、代表题练习与复盘为准。

上级导航：[1.4 STL 与竞赛常用 C++ 工具](../README.md)；横向对照：[Containers 索引与选型](../containers/README.md)。
