# 树状数组：增量与前缀和

实现：[fenwick.hpp](fenwick.hpp)。原理与两题复盘见 [9.4 树状数组](../../notes/09-data-structures/04-fenwick-tree.md#9-典题与复盘)。

**默写只记两个方向：add 向上加 lowbit，sum 向前减 lowbit。** 位运算直接写进循环，保留一个 BIT，不另造差分专用类。

## 接口与约定

| 接口 | 行为 |
| --- | --- |
| `BIT(n)` | 创建 n 项全零数据；每组重新构造 |
| `add(x,v)` | 第 x 项增加 v，不是赋值；合法位置为 1..n |
| `sum(x)` | 当前维护对象的前 x 项之和，允许 x=0 |
| `sum(l,r)` | 当前维护对象闭区间 [l,r] 的和 |

约定 $0\le n<2^{30}$ 且内存足够；增量、差分、块和、求和及相减的中间结果均可用 ll 表示。**add 不能传 0；sum 不能传负下标或超过 n 的下标。**

仅为差分末尾标记约定 `add(n+1,v)` 是空操作，循环不进入，也不访问 bit[n+1]；不要因此把越界查询当成合法。

构造后仍需逐项 add 初值。更新只改变内部 bit，不修改外部数组；`bit[i]` 是局部块和，不能直接当成原值或完整前缀。

## 同一接口，两种维护对象

```cpp
#include "fenwick.hpp"

// 维护原数组：单点加、区间和
BIT a(5);
a.add(2, 7);
ll interval = a.sum(2, 4); // 7

// 维护差分：此处原数组初始全为 0
BIT d(5);
d.add(2, 3);              // 原数组从位置 2 开始增加 3
d.add(5 + 1, -3);         // [2,5] 加 3；末尾标记自然跳过
ll value = d.sum(4);      // 原数组第 4 项为 3
```

原数组非零时，差分树应先加入相邻差值，不能只记录后续操作再把查询结果当成完整原值。区间加的 `-v` 以及构建时的 `a[i]-a[i-1]` 也要在 ll 范围内计算。

建零树时间与空间 O(n)，逐项 add 建树 O(n log n)，单点增加、前缀和及闭区间和均为 O(log n)。

## 写法取舍与来源

根据 [P3374、P3368 的实际实现](../../notes/09-data-structures/04-fenwick-tree.md#9-典题与复盘)自行收敛：合并 lowbit 辅助函数到循环，以 `sum(x)`、`sum(l,r)` 缩短接口；教学笔记继续保留展开过程与原实现。

接口比较参考 [ACL](https://github.com/atcoder/ac-library/blob/master/atcoder/fenwicktree.hpp) 与 [KACTL](https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/FenwickTree.h)。它们的外部 0-based／半开区间约定不直接混用；本模板保留 **外部 1-based、闭区间、ll 累计**，未移植上游代码，也不增加模板参数、排名查找或私人宏依赖。

## 核验状态

**C++11 本地核验通过，编译无警告。** 已与普通数组对照两种维护对象、正负增量、空前缀、单元素、闭区间端点、n+1 空更新及累计超过 int 的情况；并核验独立构造、查询不改状态和大规模输入。平台结果与个人独立复现状态另记，模板存在不代表已经熟练。
