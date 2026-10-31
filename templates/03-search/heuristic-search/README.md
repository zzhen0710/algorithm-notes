# 启发式搜索：A* 与 IDA*

C++11，实现：[astar.hpp](astar.hpp)、[idastar.hpp](idastar.hpp)。每步代价为 1，h 为固定、非负且不高估的剩余步数下界。

```cpp
#include "astar.hpp"
#include "idastar.hpp"

std::vector<std::vector<int>> g = {{1, 2}, {3}, {3}, {}};
auto goal = [](int u) { return u == 3; };
auto next = [&](int u) { return g[u]; };
auto h = [](int u) { return u == 3 ? 0 : 1; };

auto a = astar_path(0, goal, next, h);
auto b = idastar_path(0, goal, next, h);
// 返回含起终点的最短路径，步数均为size()-1，不承诺是同一条并列路径。

// 可选第五参：只允许最多两步。原有五参数调用继续可用。
auto limited = astar_path(0, goal, next, h, 2);
```

**用途：** 写好状态、Goal、Step、Heuristic，就可以互换两个函数调用；一个用全局小根堆，一个用 f 门槛下的 DFS。默认 `max_steps=-1`，不设人为上限；例如 P1379 有限且保证可达，无须知道 31 这个背景上界。

| 约定 | 说明 |
| --- | --- |
| State | 可复制、赋值，支持 hash 和 ==；包含决定后续所需的全部信息，不要求大小比较或默认构造 |
| 回调 | goal 判断目标，next 返回有限个一步后继，h 不高估到最近合法目标的距离，目标处为0；规则固定 |
| 返回 | 路径含起终点，零步成功是单元素vector；空vector表示耗尽或给定预算内无解 |
| 可选上限 | 默认-1；若给非负值，则限制最多步数。g、h及下一层步数须在int范围内；实际f须小于INT_MAX，为哨兵留值 |
| 清理 | 数据局部创建，重复调用无需清理，不修改调用方图 |

默认用于有限可达状态图；若无限状态图又无解，不设预算时不承诺终止。IDA* 的实际递归深度须适合调用栈。

- **A***：按 f=g+h 取候选，best_g 允许更短路线重开，旧记录跳过，目标有效记录出堆才返回；只要求可采纳，不额外要求一致性。
- **IDA***：当前路径防环，失败后撤销；bound 从 h(start) 起步，下轮取最小超限 f，没有新门槛时结束。

时间取决于估价与展开量，IDA* 可能重复搜索；A* 保存堆／距离／前驱，IDA* 主要保存路径／栈，生成的邻居列表也占空间。另计哈希、复制、估价与生成成本，最坏仍可能指数增长。

正文固定 string，模板接受其他 hashable State；策略相同。分步图、对比动画与估价设计与八数码实际代码见 [3.7 启发式搜索](../../../notes/03-search/07-heuristic-search.md)。

[返回本章](../README.md)
