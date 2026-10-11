# 14 · swap / iter_swap 与元素交换

这一节介绍两个常用的交换工具：

| 工具 | 操作对象 | 作用 |
| --- | --- | --- |
| `swap` | 两个对象 | 交换对象的值 |
| `iter_swap` | 两个迭代器 | 交换迭代器所指元素的值 |

核心：

> `swap` 接收元素，`iter_swap` 接收位置；交换的都是值。

本节以 **C++11** 为兼容基准。示例所需头文件：

```cpp
#include <utility>   // std::swap：C++11 中的通用交换工具
#include <algorithm> // std::iter_swap：交换两个迭代器所指元素
#include <vector>    // std::vector：示例使用的动态数组
```

---

# 一、swap

## API

```cpp
// 通用版本的简化声明，省略条件 noexcept；返回类型为 void。
// T 必须满足该版本所需的移动构造与移动赋值要求。
template<class T>
void swap(
    T& a,  // 第一个可修改对象
    T& b   // 第二个可修改对象，通用版本要求两者为同一类型
);
```

作用：交换 `a` 与 `b` 的值。

```cpp
// 示例 1：交换两个整数；代码片段放入 main() 中运行。
int x = 3;
int y = 8;
std::swap(x, y);
// 结果：x = 8，y = 3。
```

交换数组中的两个元素：

```cpp
// 示例 2：直接通过下标取得需要交换的元素。
std::vector<int> a = {10, 20, 30};
std::swap(a[0], a[2]);
// 结果：a = {30, 20, 10}。
```

对于整数，可以将交换过程理解为“临时保存一个值，再完成两次赋值”。通用 `std::swap` 通常借助移动操作实现；自定义类型和容器也可能有专门的交换实现。

---

# 二、iter_swap

## API

```cpp
// 两个迭代器可以是不同类型，但所指元素必须能够相互交换。
// 返回类型为 void；交换元素的值，不交换迭代器本身。
template<class ForwardIt1, class ForwardIt2>
void iter_swap(
    ForwardIt1 a, // 指向第一个可修改元素的有效迭代器
    ForwardIt2 b  // 指向第二个可修改元素的有效迭代器
);
```

作用：交换 `a`、`b` 所指元素的值。

```cpp
// 示例 3：交换迭代器所指元素，迭代器仍指向原来的位置。
std::vector<int> a = {1, 2, 3, 4};
auto left = a.begin();
auto right = a.begin() + 3;

std::iter_swap(left, right);
// 结果：a = {4, 2, 3, 1}。
// left 仍是 a.begin()，但 *left 变为 4；*right 变为 1。
```

两个迭代器不必来自同一个容器，只要它们有效且所指元素可以交换。

---

# 三、与 swap 的关系

对于普通 `vector<int>` 迭代器，`std::iter_swap(it1, it2)` 可以按 `std::swap(*it1, *it2)` 理解。

但要区分下面三个操作：

| 写法 | 交换什么 |
| --- | --- |
| `std::swap(a[i], a[j])` | 两个下标位置的元素值 |
| `std::iter_swap(it1, it2)` | 两个迭代器所指元素的值 |
| `std::swap(it1, it2)` | 两个迭代器变量保存的位置 |

```cpp
// 示例 4：交换迭代器变量，只改变指向，不改变容器内容。
std::vector<int> a = {1, 2, 3};
auto it1 = a.begin();
auto it2 = a.begin() + 2;

std::swap(it1, it2);
// a 仍为 {1, 2, 3}；现在 *it1 == 3，*it2 == 1。
```

对于代理引用等特殊迭代器，不应认为 `iter_swap` 与显式调用 `std::swap(*it1, *it2)` 在所有类型上都可以直接互换。泛型算法中需要交换迭代器所指元素时，优先表达为 `iter_swap`。

---

# 四、常见应用与注意事项

## 1. 双指针交换

反转序列的核心是从两端向中间不断交换元素。

```cpp
// 示例 5：用两个下标手写反转，空 vector 也可以安全处理。
std::vector<int> a = {1, 2, 3, 4, 5};
int left = 0;
int right = static_cast<int>(a.size()) - 1; // 示例假设 size 可用 int 表示

while (left < right) {
    std::swap(a[left], a[right]); // 交换两端元素
    ++left;
    --right;
}
// 结果：a = {5, 4, 3, 2, 1}。
```

实际需要反转整个区间时，可直接使用前面学过的 `reverse`。交换也常出现在排序、划分和排列生成中。

## 2. 位置必须有效，元素必须可修改

- `end()` 是尾后位置，不能解引用，因此不能传给 `iter_swap` 交换元素。
- `a.begin() + k` 要求迭代器支持随机访问，并且结果位置有效；不是所有容器都支持这种写法。
- `const_iterator` 通常不能用于交换所指元素；`set` 的键等只读元素也不能直接交换。
- 交换整个容器与交换容器内两个元素是不同操作。

---

# 五、复杂度与本节总结

| 操作 | 时间复杂度 | 说明 |
| --- | --- | --- |
| 交换两个整数 | $O(1)$ | 常数次基本操作 |
| `iter_swap` | 一次元素交换的开销 | 元素为整数时通常为 $O(1)$ |
| 交换两个同长度内置数组 | $O(n)$ | 逐项交换，$n$ 为数组长度 |
| 交换两个普通 `vector<int>` 容器 | $O(1)$ | 默认分配器下交换容器内部管理状态 |

不要把所有类型的 `swap` 都记成 $O(1)$：实际开销取决于对象类型及其交换实现。

本节重点：

> 手上是两个元素，用 `swap`；手上是两个迭代器，需要交换所指元素，用 `iter_swap`。

接口核对：[N3337：swap](https://timsong-cpp.github.io/cppwp/n3337/utility.swap)、[C++ 标准工作草案：交换算法](https://eel.is/c++draft/alg.swap)。本节 API 按 C++11 写法简化。
