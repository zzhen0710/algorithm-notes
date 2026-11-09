# 随机快速排序

`randomized_quick_sort(a, l, r, rng)` 原地升序排序整数数组的闭区间 `[l,r]`。非空区间的下标必须有效，空数组可传 `[0,-1]`。随机数引擎由调用者创建一次后复用。

```cpp
std::mt19937 rng(std::random_device{}());
randomized_quick_sort(a, 0, (int)a.size() - 1, rng);
```

双向扫描在等值处停止，交换后两端一起推进。期望 $O(n\log n)$，最坏 $\Theta(n^2)$，递归栈最坏 $O(n)$。

[实现](randomized_quicksort.hpp) · [划分与复杂度说明](../../../notes/10-supplementary/03-randomized-quicksort.md) · [本章索引](../README.md)
