# 15 · Lambda 与比较器总结

前面的排序、筛选、划分和映射算法，经常需要传入一个函数。本节集中整理 **C++11 Lambda** 的写法、捕获方式和比较器规则。

核心：

> 算法负责遍历和组织数据，Lambda 负责说明“怎样比较、怎样判断、怎样转换”。

示例所需头文件如下，后续片段可分别放入 `main()` 中运行：

```cpp
#include <algorithm> // sort、stable_sort、count_if、partition、transform
#include <numeric>   // iota：生成连续编号
#include <vector>    // vector：示例容器
#include <utility>   // pair：多关键字数据
#include <functional> // function：保存工厂返回的可调用对象
```

---

# 一、Lambda 的基本写法

Lambda 是一种就地定义函数对象的简洁写法。

```cpp
// 示例 1：捕获列表为空，接收两个整数，显式返回 int。
auto add = [](int x, int y) -> int {
    return x + y; // 示例中的整数较小，结果不会溢出
};
int result = add(2, 3); // result = 5
```

| 部分 | 示例 | 含义 |
| --- | --- | --- |
| 捕获列表 | `[]` | 是否使用外部局部变量，以及怎样保存它们 |
| 参数列表 | `(int x, int y)` | 调用时接收的参数 |
| 返回类型 | `-> int` | 指定返回值类型；简单情况可省略 |
| 函数体 | `{ return x + y; }` | 实际执行的操作 |

简单的单条 `return` 通常可以推导返回类型。为了兼容 C++11，本节遇到多分支返回时显式写出返回类型；参数也写明类型，`[](auto x)` 是 C++14 的泛型 Lambda 写法。

---

# 二、捕获：[]、[=] 与 [&]

参数来自函数调用，捕获来自 Lambda 定义处的外部局部变量。

| 写法 | 含义 |
| --- | --- |
| `[]` | 不捕获外部局部变量 |
| `[x]` | 按值捕获 `x`，保存创建 Lambda 时的副本 |
| `[&x]` | 按引用捕获 `x`，访问原变量 |
| `[=]` | 默认按值捕获函数体需要使用的外部局部变量 |
| `[&]` | 默认按引用捕获函数体需要使用的外部局部变量 |
| `[x, &y]` | 按值捕获 `x`，按引用捕获 `y` |

```cpp
// 示例 2：对比创建时的副本与调用时的原变量。
int limit = 10;
auto by_value = [limit](int x) { return x >= limit; }; // 保存 10
auto by_ref = [&limit](int x) { return x >= limit; };  // 引用 limit

limit = 20;
bool first = by_value(15); // true：仍与 10 比较
bool second = by_ref(15);  // false：现在与 20 比较
```

引用捕获也可以修改外部变量，但必须保证被引用变量在 Lambda 调用时仍然存活。不要返回一个引用捕获了函数内部局部变量的 Lambda，再在函数结束后调用它。

按值捕获默认不能修改捕获的副本；加 `mutable` 可以修改副本，但不会因此修改外部原变量。竞赛中优先使用清楚、最小的捕获列表，避免无意复制大型容器。

## 函数工厂：先固定规则，再返回函数

```cpp
using Test = std::function<bool(int)>; // 需要 <functional>

Test make_at_least(int limit) {
    return [limit](int x) -> bool {
        return x >= limit;
    };
}

// 以下调用放在 main 或其他函数内。
auto at_least_3 = make_at_least(3);
auto at_least_8 = make_at_least(8);
bool a = at_least_3(5); // true
bool b = at_least_8(5); // false
```

**用途速记：** 先把固定条件交给工厂，得到一个保存该条件的函数对象；以后调用它时，再提供每次变化的数据。两次创建使用相同代码，但可以保存不同的捕获值。

`make_at_least(3)` 现在执行工厂，创建并返回一个记住 3 的对象；`at_least_3(5)` 才真正执行判断。`[limit]` 保存副本，工厂返回后对象仍有自己的条件。

按值捕获让返回对象保存数据；引用捕获不会替原数据延长生命周期，必须确保调用时原数据还存在。捕获变量本身是对象引用时，按值捕获会复制它所引用的对象，参见 [C++11 草案的按值捕获规则](https://timsong-cpp.github.io/cppwp/n3337/expr.prim.lambda#14)。

实际应用见 [P1032 的 make_step](../../../03-search/05-bidirectional-bfs.md#工厂调用和真正执行一步是两个时刻)：分别固定正向与反向规则，生成两个 `Step`，再交给同一份搜索主体。

---

# 三、比较器：谁应该排在前面？

对于 `sort / stable_sort / partial_sort / nth_element`，可以把 `comp(a, b)` 理解为：

> 按当前规则，a 是否应严格排在 b 前面？

```cpp
// 示例 3：true 表示第一个参数应该排在第二个参数之前。
std::vector<int> a = {3, 1, 2};

std::sort(a.begin(), a.end(), [](int x, int y) {
    return x > y; // 较大的数排在前面，即降序
});
// a = {3, 2, 1}；改成 x < y 即为升序。
```

## 多关键字比较

例如：第一项降序，第一项相同时第二项升序。

```cpp
// 示例 4：两个关键字分别判断，主关键字相等后才比较次关键字。
std::vector<std::pair<int, int>> a = {{3, 20}, {5, 30}, {3, 10}};

std::sort(a.begin(), a.end(),
    [](const std::pair<int, int>& x,
       const std::pair<int, int>& y) -> bool {
        if (x.first != y.first) {
            return x.first > y.first; // 第一项降序
        }
        return x.second < y.second;   // 第二项升序
    }
);
// a = {{5, 30}, {3, 10}, {3, 20}}。
```

使用 `const T&` 可以避免不必要的对象复制，并明确比较过程中不修改元素。若所有字段都按字典序升序排列，`pair / tuple` 的默认比较通常已经够用。

---

# 四、严格弱序与稳定性

## 1. 比较器必须满足严格弱序

排序算法要求比较规则自洽，重点检查：

- **自己不小于自己**：`comp(a, a)` 必须为 false。
- **不能相互排在前面**：`comp(a, b)` 为 true 时，`comp(b, a)` 必须为 false。
- **比较关系可传递**：a 在 b 前、b 在 c 前，就应有 a 在 c 前。
- **等价关系可传递**：若两边比较都为 false，就视为比较器等价，这种等价也必须一致、可传递。

因此升序写 `<`，降序写 `>`，不能写 `<=` 或 `>=`。违反要求会导致未定义行为，不能只当作“排序结果可能不对”。

不要通过整数相减的正负号判断大小，减法可能溢出；也不要在排序期间修改比较器依赖的外部数据。

## 2. stable_sort 保留的是比较器等价元素的顺序

```cpp
// 示例 5：只按分数降序；同分者在比较器意义下等价。
struct Student {
    int score; // 分数
    int id;    // 输入编号
};
std::vector<Student> a = {{90, 1}, {80, 2}, {90, 3}};

std::stable_sort(a.begin(), a.end(),
    [](const Student& x, const Student& y) {
        return x.score > y.score; // 同分时两个方向都返回 false
    }
);
// 编号顺序为 1、3、2；同分的 1 和 3 保持原相对顺序。
```

`sort` 不保证这种保序性质。若题目规定同分后按编号升序，应把编号写进比较器，不能仅凭稳定排序来满足任意平局规则。

---

# 五、同一写法，承担不同角色

| 角色 | 典型算法 | 函数返回值的含义 |
| --- | --- | --- |
| 比较器 | `sort / nth_element` | 第一个元素是否严格排在第二个之前 |
| 一元谓词 | `count_if / copy_if / partition` | 当前元素是否满足条件 |
| 转换函数 | `transform` | 当前元素转换后的新值 |
| 聚合函数 | `accumulate` | 当前累计值与新元素合并后的结果 |

```cpp
// 示例 6：谓词用于计数和划分，转换函数用于生成新值。
std::vector<int> a = {1, 2, 3, 4};
auto even = [](int x) { return x % 2 == 0; }; // true 表示偶数

auto count = std::count_if(a.begin(), a.end(), even); // count = 2
auto middle = std::partition(a.begin(), a.end(), even);
// [a.begin(), middle) 是偶数；其余是奇数，各部分内部顺序不保证。

std::vector<int> b(a.size());
std::transform(a.begin(), a.end(), b.begin(), [](int x) {
    return x * 2; // 返回转换结果，不是返回是否保留元素
});
// b 中元素按当前 a 的位置逐项翻倍。
```

谓词为 true 的含义由算法决定：`copy_if` 复制它，`remove_if` 移除它，`partition` 把它放到前半段。

---

# 六、经典技巧：捕获原数据，排序编号

```cpp
// 示例 7：保持原数据不动，只改变编号数组的顺序。
std::vector<int> value = {80, 95, 70, 95};
std::vector<int> id(value.size());
std::iota(id.begin(), id.end(), 0); // id = {0, 1, 2, 3}

std::sort(id.begin(), id.end(), [&value](int x, int y) -> bool {
    if (value[x] != value[y]) {
        return value[x] > value[y]; // 数据较大的编号排在前面
    }
    return x < y;                  // 数据相同时按原编号升序
});
// id = {1, 3, 0, 2}；value 仍为 {80, 95, 70, 95}。
```

这里参数 `x / y` 是待比较的编号，捕获的 `value` 是比较依据。排序过程中只交换编号，`value` 必须保持稳定。

---

# 七、本节总结

- 不用外部局部变量时写 `[]`；需要外部数据时明确按值还是按引用捕获。
- 比较器回答“谁严格排在前面”，谓词回答“是否满足条件”，转换函数返回“新值”。
- 排序比较器使用严格比较，处理好次关键字和等价元素。
- 需要保留比较器等价元素的原顺序时，使用 `stable_sort`。
- Lambda 越短、捕获越明确，越容易检查正确性；复杂逻辑可以拆成命名函数。

接口与规则核对：[N3337：Lambda](https://timsong-cpp.github.io/cppwp/n3337/expr.prim.lambda)、[N3337：排序与比较关系](https://timsong-cpp.github.io/cppwp/n3337/alg.sorting)。
