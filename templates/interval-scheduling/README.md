# 区间选择：最多互不冲突区间

按结束时间排序，维护最后已选区间的结束边界。来源、交换论证与两题实际实现见 [4.2 区间贪心](../../notes/04-greedy-and-construction/02-interval-greedy.md#8-典题与复盘)。使用 C++11。

实现：[interval_scheduling.hpp](interval_scheduling.hpp)。本模板只适用于**等收益、最大数量的区间选择**，不适用于带权调度或区间合并。

## 接口与约定

- `int max_nonoverlap(vector<Interval> a, bool allow_touch = true)`：返回最多选择条数，不返回具体方案。
- `Interval` 的 `l、r` 使用 `ll`，支持负数及有符号 64 位端点；只比较端点，不做加减。
- `allow_touch=true`：非空半开区间 $[l,r)$，要求 `l<r`，允许首尾相接。
- `allow_touch=false`：闭区间 $[l,r]$，要求 `l<=r`，共享端点冲突，单点区间合法。
- 容器位置 **0-based**，条数可用 `int` 表示；全体输入必须采用同一种端点语义。
- 按值接收并排序副本，**原数组不变**；空输入返回 `0`，每次调用独立初始化。

## 使用示例

```cpp
#include "interval_scheduling.hpp"
#include <iostream>

int main() {
    std::vector<Interval> a = {{1, 3}, {3, 5}};

    std::cout << max_nonoverlap(a) << '\n';        // 2，允许相接
    std::cout << max_nonoverlap(a, false) << '\n'; // 1，共享端点冲突
}
```

## 边界、成本与来源

**跳过区间时不更新 `last_r`；`noempty` 避免负坐标被初始 `0` 排除。** 同结束时间按起点排序不是正确性所必需，不能把比较器改成 `<=`。

排序 $O(n\log n)$，扫描 $O(n)$，复制输入额外保存 $O(n)$ 个区间。需要原地排序时可按任务改成引用入口；这会改变副作用契约。

来自两题实际使用的 `max_nonoverlap`，整理接口将端点从 `int` 扩为 `ll`，并用参数统一端点规则；不是两份原提交代码的原样副本。两题平台结果未单独记录，模板及两题实现已完成本地编译、暴力对照与边界核验。
