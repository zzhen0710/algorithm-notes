# 双向 BFS：等代价状态图的两端搜索

实现：[bidirectional_bfs.hpp](bidirectional_bfs.hpp)。C++11，整层扩展，优先当前层较小的一端，自行整理；不包含棋盘或字符串替换的专属规则。

```cpp
#include "bidirectional_bfs.hpp"

std::vector<std::vector<int>> g = {{1, 2}, {3}, {3}, {}};
std::vector<std::vector<int>> rg(g.size());
for (int u = 0; u < (int)g.size(); ++u)
    for (int v : g[u]) rg[v].push_back(u);

auto next = [&](int u) { return g[u]; };
auto prev = [&](int u) { return rg[u]; };
int distance = bidirectional_bfs(0, 3, next, prev); // 2
int limited = bidirectional_bfs(0, 3, next, prev, 1); // -1
```

**用途：** 给出指定起终点与正向后继、反向前驱生成规则，返回等代价最少步数；可传总步数上限。相遇时将两侧到同一状态的距离相加，整层结束后返回本轮最短拼接值。

| 接口／契约 | 说明 |
| --- | --- |
| `State` | int、string 等可复制、支持 `std::hash<State>` 和 `==` 的类型；两端类型相同，字符串字面量先转为 string |
| `next(u)` | 返回原图全部合法后继，能用于范围 for 的候选集合 |
| `prev(u)` | 返回原图全部合法前驱，不是随意复用后继；无向且规则对称时可同一函数 |
| 图与代价 | 每条边恰为一步；生成规则固定，完整状态不应编码成同一键，普通哈希碰撞由相等比较处理；每次候选有限 |
| `max_steps` | 负数不限；非负则是整条路径的总步数上限，默认 −1；步数及两端深度和可用 int 保存 |
| 返回 | 非负为最少步数；起终点相同为 0；−1 表示限制内无解，不限且耗尽时表示不可达 |
| 生命周期 | 两侧层、距离表均局部创建，多次调用无需清理；算法不改调用方状态或图，生成函数的副作用由调用方负责 |
| 终止前提 | 无限制时可达状态有限；无限图可能不结束，有限总步数限制也不保证状态数很少 |

`bidir_detail::layer` 是内部整层操作，通常只调用 `bidirectional_bfs`。

**三项职责：** 调用方的 `next/prev(u)` 只生成一个状态的一步邻居；内部 `layer` 汇总选中一侧的完整层、记录距离并检查相遇；主函数调度两端。不同题目主要调整状态与生成规则，正反规则可以相同也可以不同。详见[通俗职责说明](../../../notes/03-search/05-bidirectional-bfs.md#三个函数各负责哪一层工作)。教学版固定 string，使用 `std::function`；模板用类型参数接受 int/string 等和不同 lambda，状态与终止语义一致。

**复杂度：** 整数有限图按哈希平均常数计，平均 $O(V+E)$ 时间，距离表与层占 $O(V)$；反图、生成器与临时候选列表另计。隐式图按实际状态、候选与生成代价统计；字符串的哈希和复制也需计入。接近均匀树状扩展时可减少到半深度量级，不保证所有图都有指数收益。

不返回具体路径，不处理一般非等权边，不代替从单源到全部点的 BFS。总步数限制与整层相遇的证明、反例及执行图见 [3.5 双向 BFS](../../../notes/03-search/05-bidirectional-bfs.md)。


正图／反图怎样由同一组边生成，以及两个回调怎样读取邻接表，见[具体建图与调用](../../../notes/03-search/05-bidirectional-bfs.md#具体建图与调用)。

两题的具体生成函数复盘：[P1379 棋盘移动](../../../notes/03-search/05-bidirectional-bfs.md#91-p1379八数码难题)与[P1032 规则工厂](../../../notes/03-search/05-bidirectional-bfs.md#92-p1032字串变换)。规则实现留在题目，通用模板主体继续复用，不为两题再复制两份搜索。
