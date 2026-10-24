# 线段树：点修改与懒标记

来源笔记：[9.6 基础线段树](../../../notes/09-data-structures/06-segment-tree-basics.md)、[9.7 懒标记](../../../notes/09-data-structures/07-lazy-segment-tree.md)。

**按所需操作选一个头文件，不必四份一起默写。** 每种已验证模型保留一份主写法，不做需要背更多接口的通用运算框架。

## 实现与接口

| 头文件／类型 | 操作 | 维护信息 |
| --- | --- | --- |
| [segment_max.hpp](segment_max.hpp)／`SegMax` | `set(x,v)`、`query(l,r) const` | 点赋值、区间最大值 |
| [lazy_add_sum.hpp](lazy_add_sum.hpp)／`LazyAddSum` | `add(l,r,d)`、`query(l,r)` | 区间加、区间和 |
| [lazy_affine_sum.hpp](lazy_affine_sum.hpp)／`LazyAffineSum` | `multiply(l,r,c)`、`add(l,r,d)`、`query(l,r)` | 乘加与区间和取模 |
| [lazy_flip_count.hpp](lazy_flip_count.hpp)／`LazyFlipCount` | `flip(l,r)`、`query(l,r)` | 翻转 01、区间 1 的数量 |

前三种从 `vector<ll> a` 构造，`a[0]` 占位、有效数据为 `a[1..n]`。模意义版本构造为 `LazyAffineSum(a,mod)`；其余两种为 `Type(a)`。

**翻转版本直接接收普通字符串 `s`，不补空格。** 原字符串为 0-based，但全部操作端点仍是 1-based，叶子读取 `s[l-1]`。

## 公共契约

- `1≤n≤2×10⁵`，内存足够；所有查询／区间更新都是非空闭区间，满足 `1≤l≤r≤n`，点赋值满足 `1≤x≤n`。
- 构造不改输入，后续操作也不同步外部数组或字符串。每组数据重新构造，各对象独立；无空结构、越界、非法字符等防御分支。
- **点赋值是直接替换**。P1531 的“只允许提高”由调用处判断，类本身允许降低。
- `SegMax` 返回 ll，支持整个 ll 值域；用最小可表示值作查询单位元。`LazyFlipCount` 返回 int，计数不超过 n。
- `LazyAddSum` 允许正、负、零增量；要求每个节点和、标记、长度乘积及查询累加的所有中间值都可用 ll 表示，不能仅检查最终输出。
- 懒标记版本的 query 可能下传，改变内部保存位置，但不改变数组逻辑值，**因此不加 const**；普通最大值查询只读。
- 建树／空间 `O(n)`，每次操作 `O(log n)`；完整覆盖根时无需继续细分。

## 模意义乘加：接口与数值范围

**要求 `1≤mod≤10⁹+9`。** 初值、传入 transform／add／multiply 的系数可为任意 ll，入口统一规范到 `[0,mod)`；返回结果也在此范围。

`transform(l,r,c,d)` 表示对区间每项做 `x→cx+d`，是 add／multiply 共用的入口。需要整段赋值 v 时，直接 `transform(l,r,0,v)`，不再另写一套递归。

归一化后，apply 的最大乘加量小于 `mod²+mod×n`；在本模板的 n、mod 上限下可用 ll 表示。**扩大模数范围时必须重新检查，取模之前仍可能溢出**。

`tm、ta` 表示待下传的乘法／加法参数；新操作接在旧结果之后。恒等标记为 `(1%mod,0)`，所以 mod=1 时也不需要反复下传无效标记。内部辅助函数接收已规范化的参数；调用者使用公开的简短接口，不绕过入口直接写 tr／标记或调用递归函数。

## 简短使用

```cpp
#include "lazy_affine_sum.hpp"

std::vector<ll> a = {0, 1, 2, 3};
LazyAffineSum seg(a, 1009);
seg.add(1, 3, 3);
seg.multiply(1, 3, 2);
ll total = seg.query(1, 3); // 30；先加后乘，旧加数也要乘 2。
seg.transform(2, 3, 0, -1); // 第 2、3 项赋为 -1 的模内代表元 1008。
```

## 与教学实现的差异

自行从已核验笔记提炼，**没有移植外部库代码**。变量与重复分支适度缩短，保留 apply／push／pull 的必要过程注释。

模意义模板把原 `multi/add` 的重复递归合为 update，入口额外处理负系数与规范化；`multiply` 对应原笔记的 `multi`。翻转模板查询直接返回 int。笔记仍保留真实原实现与展开讲解，不反向替换个人解法。

**C++11 本地核验通过，编译无警告。** 四个头文件独立及与既有头文件共同包含均通过；与逐项扫描、独立分块参照核对更新和查询，覆盖顺序复合、抵消、负系数、模数 1、大数与最大规模。具体执行记录留在本地计划；验证不代表平台 AC 或已经熟练。

[本章模板](../README.md) · [全部模板](../../README.md) · [操作复合范式](../../../patterns/09-data-structures/composable-transformations.md)
