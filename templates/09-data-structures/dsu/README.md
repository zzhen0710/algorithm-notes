# 并查集：路径压缩与按大小合并

实现：[dsu.hpp](dsu.hpp)。原理及配题见 [9.1 并查集](../../../notes/09-data-structures/01-dsu.md)，在 [6.3 Kruskal](../../../notes/06-graph-and-tree/03-kruskal-mst.md)中复用。

## 来源与适配

主写法取自 [ei1333/library 的 UnionFind](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/structure/union-find/union-find.hpp)，版本 `088b4c8b49f85194099305d998e7113dc3d5dd2e`，原项目采用 [Unlicense](https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/README.md#-license)。

保留负值数组、路径压缩、按大小合并和 `bool unite`；适配为 **1-based**，增加 `components`，省去当前不需要的 `groups`。该独立实现使用 C++11，不引入上游整套框架。

## 状态与接口

`p` 按对象数分配 `n+1` 项。**同一项的含义取决于它是不是根：**

| 状态 | `p[x]` 的含义 |
| --- | --- |
| `p[x] < 0` | `x` 是根，`-p[x]` 为整个集合大小 |
| `p[x] > 0` | `x` 不是根，`p[x]` 为父节点编号 |
| `p[0] = 0` | 无效占位，不允许查询或合并编号 0 |

合并两个单点后，根处从 `-1` 变成 `-2`，另一个点改为存根编号。**根处比较负大小，判断方向与比较普通大小相反。**

| 接口 | 返回值与行为 |
| --- | --- |
| `DSU(n)` | 创建 `n` 个单点集合，允许 `n=0`，此时没有合法查询 |
| `find(x)` | 当前根；途中压缩父指针 |
| `unite(a,b)` | 不同块则合并并返回 `true`；已同块返回 `false` |
| `same(a,b)` | 是否同组，可能压缩父指针 |
| `size(x)` | `x` 所在集合的大小 |
| `components` | 当前集合数，只在成功合并时减一 |

本接口与 [ACL 的 `merge`](https://atcoder.github.io/ac-library/production/document_en/dsu.html)不同：后者返回根编号，不能直接当成本模板的成功标记使用。

## 使用与边界

```cpp
#include "dsu.hpp"

DSU d(4);
bool added = d.unite(1, 2); // true，集合数从 4 变为 3
bool repeated = d.unite(2, 1); // false，不再减少集合数
bool connected = d.same(1, 2); // true
int members = d.size(2); // 2，先找到根再读取大小
```

要求 `0<=n<=10^6`，每次操作编号均在 `1..n`；不会把非法编号解释为“不连通”。每组数据重新构造对象，多测不残留状态。

父指针只表示并查集内部关系，不能当作原图路径。普通并查集不支持直接删边，也不保存异色、距离等额外约束。

初始化时间与空间为 $O(n)$；合并、查根等操作的均摊时间为 $O(\alpha(n))$。

## 验证状态

来源与接口已核对，**C++11 本地核验通过，编译无警告**。上游验证与本地适配后的验证分别记录；已按独立分组标记检查合并返回值、同组关系、集合大小与数量，并核验重复/自身合并、空初始化、路径压缩与大规模初始化。模板存在不表示个人已经熟练。

[本章模板](../README.md) · [全部模板](../../README.md)
