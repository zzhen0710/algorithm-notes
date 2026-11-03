# 整数平面几何：距离、方向、线段、凸形内点与面积

实现：[geometry_int.hpp](geometry_int.hpp)。代码与图解见 [7.12 基础几何](../../../notes/07-math/12-basic-geometry.md)，原理参考 cp-algorithms，本实现按下列契约自行整理。

```cpp
#include "geometry_int.hpp"

geom::Point a{1,1}, b{5,1}, c{3,3};
auto side = geom::orient(a, b, c); // 8，在 a→b 左侧。
bool on = geom::on_segment({3,1}, a, b); // true，含端点。
bool hit = geom::segments_intersect(a, b, {3,0}, {3,2}); // true。
std::vector<geom::Point> p{{2,2},{4,2},{4,4},{2,4}};
auto area2 = geom::polygon_area2(p); // 8，实际面积为 4。
bool inside = geom::point_in_convex(p, {3,3}); // true，默认含边界。
bool strict = geom::point_in_convex(p, {2,3}, false); // false，不含边界。
```

**用途速记：** 给出整数点坐标，按需取得方向、距离平方、闭线段位置关系、凸图形内点判断和简单多边形两倍面积。判定用整数乘加；不维护全局状态，不修改输入。

| 接口 | 返回含义 |
| --- | --- |
| `Point {x,y}`、点相减 | ll 坐标；相减得到位移向量 |
| `dot(u,v)`、`cross(u,v)` | 两个向量的点积、二维叉积，ll |
| `orient(a,b,c)` | a→b 到 a→c 的叉积；正左负右，0 共线；a=b 无有效方向 |
| `dist2(a,b)`、`distance(a,b)` | 距离平方 ll、距离 double |
| `on_segment(p,a,b)` | p 是否在闭线段 ab 上，包括端点 |
| `segments_intersect(a,b,c,d)` | 闭线段 ab 与 cd 是否有公共点 |
| `point_in_convex(poly,p,border=true)` | p 是否在凸多边形内，border 控制是否接纳边界 |
| `polygon_area2(p)` | 非负的两倍面积，ll；少于 3 点为 0 |
| `sgn(x)` | 把 ll 值压为 -1/0/1，避免直接相乘判异号 |

## 前提与边界

- 输入点的 x、y 都为整数，绝对值不超过 1000000；向量允许是上述点的一次坐标差。不能任意叠加向量后仍直接沿用数值界。
- polygon_area2 的多边形顶点数不超过 200000；按边界顺／逆时针排列，简单且不自交，允许凹形；函数不负责检验输入是否满足这些条件。下标为 0..n-1，自动闭合，不要求末点重复首点。
- point_in_convex 要求至少 3 个不重复顶点、面积非零的凸多边形，沿边界顺／逆时针排列，不在末尾重复首点；允许连续共线边，不检验凸性。少于 3 点返回 false；全共线不视为有效凸图形。border=false 时只接纳严格内部。
- 点坐标 ll；差分点积／叉积／距离平方绝对值至多 8e12，面积累加保守界 4e17，在 ll 内。扩大坐标与点数时，先检查中间运算与部分和。
- 相交包含端点接触与共线重叠，允许线段退化为点；不返回交点坐标。orient 在 a=b 时返回 0，但没有“直线 ab”的意义。
- 数学坐标 y 向上，正叉积表示左侧；屏幕坐标 y 向下时视觉方向相反。
- 方向与位置判定不使用 eps；distance 是浮点近似，输出精度由调用者决定。area2 保留半整数面积，转 double 时大整数末位可能丢精度；精确 .0/.5 输出可用整除和奇偶。
- 所有函数无缓存、无多测清理要求、输入只读。namespace geom 隔离 Point、ll 等名字；头文件仅依赖标准库，兼容 C++11。
- 只处理本组基础操作，不提供实数版、交点求解、凹／任意多边形内点、凸包或扫描线。

## 复杂度与核验

凸图形内点与多边形面积为 O(n) 时间，其余接口均 O(1)；额外空间 O(1)，多边形输入另占 O(n)。两两枚举 n 条线段仍为 O(n²)。

正文与头文件分别核对有理数参数求交，覆盖交叉、平行、共线分离／重叠、端点、零长线段、端点换序与较大坐标；面积检查顺逆序、平移、凹形、空输入与顶点数边界。新增凸图形内点接口以独立射线法核对，覆盖顺逆序、边界选项、共线相邻边与坐标界。P1355／CF1030B 个人实现只静态复盘，未重新评测；通用核验不代表平台通过或个人已经掌握。

[凸图形同侧范式](../../../patterns/07-math/convex-halfplanes.md)

[本章模板](../README.md) · [本章范式](../../../patterns/07-math/README.md)
