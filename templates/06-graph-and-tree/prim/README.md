# Prim：邻接矩阵的最小生成树

实现：[prim_dense.hpp](prim_dense.hpp)。完整状态、执行图与选型见 [6.3 补充篇](../../../notes/06-graph-and-tree/03a-prim-mst.md)。

## 接口与前提

`PrimResult prim_dense(const vector<vector<ll>>& w, int s=1)`：

- **1 基**，`w` 为 `(n+1)×(n+1)` 对称矩阵，`n>=1`，`1<=s<=n`。
- 无边使用头文件中的 `PRIM_INF`；对角线可设 0，自环不参与选边。零权、负权边合法。
- 重边建矩阵时取较小值；不把 0 自动解释为“没有边”。
- 真实边权绝对值不超过 `1e12`，点数不超过 `1e6`，使累计绝对值不超过 `1e18`。实际必须另按时间与矩阵内存选择可处理点数。
- 不检查不对称矩阵、非法起点或尺寸错误；这些由调用者保证。

## 返回与使用

```cpp
#include "prim_dense.hpp"

std::vector<std::vector<ll>> w(4, std::vector<ll>(4, PRIM_INF));
for (int u = 1; u <= 3; ++u) w[u][u] = 0;
w[1][2] = w[2][1] = -1;
w[2][3] = w[3][2] = 0;
w[1][3] = w[3][1] = 5;

auto r = prim_dense(w);
if (r.ok) {
    // r.weight=-1 是合法树权重，不能拿 -1 判断失败。
    // 每个 v!=1 与 r.parent[v] 构成一条真实树边；r.parent[1]=0。
} else {
    // 原图不连通，不能读取 weight/parent 作为生成树。
}
```

| 字段 | 成功 | 失败 |
| --- | --- | --- |
| `ok` | true | false |
| `weight` | 最小生成树权重 | 0，仅占位 |
| `parent` | n+1 项，起点父节点为 0 | 空 vector |

`n=1` 返回成功、权重 0。**连通图中更换合法起点 s，最小总权值不变；同权时具体选边可以不同，parent 也随根与选边改变。** 无需枚举所有起点；这是 [MST 本身的性质](../../../notes/06-graph-and-tree/03-kruskal-mst.md#mst-的最小总权值与根无关)，Kruskal 同样求这个最优值。每次调用独立初始化，输入矩阵不变，无共享状态，多测无需清理。

## 复杂度与适用场景

时间 `O(n²)`；best、parent、in 及返回结果占 `O(n)` 空间，**调用者的矩阵另外占 O(n²)**。不依赖 DSU 或其他模板头文件，复制为比赛单文件时保留类型定义与函数即可。

适合点数适中、图稠密或已经给邻接矩阵的题。稀疏边表及指定 k 块的最小生成森林优先复用 [Kruskal](../kruskal/README.md)；不能随意提前截断 Prim 来求全局 k 块最优值。

算法自行整理，原理参考见知识笔记。通用代码核验与配题完成状态分开；[P1546 实际实现复盘](../../../notes/06-graph-and-tree/03a-prim-mst.md#p1546-最短网络-agri-net)已记录，不据此补记平台结果。

[本章模板](../README.md) · [全部模板](../../README.md)
