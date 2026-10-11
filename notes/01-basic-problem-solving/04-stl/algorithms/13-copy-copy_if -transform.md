# 13 · copy / copy_if / transform 与区间映射

这一节介绍 STL 中常用的**区间复制与转换算法**。

核心思想：

> 用输入区间说明“数据从哪里来”，用输出迭代器说明“结果写到哪里”，用函数说明“如何筛选或转换”。

它们与前面的 `fill / iota / generate` 有所不同：

| 算法 | 作用 | 数据来源 |
| --- | --- | --- |
| `fill` | 批量填入同一个值 | 指定值 |
| `iota` | 填入递增序列 | 起始值 |
| `generate` | 按规则生成数据 | 生成函数 |
| `copy` | 复制已有元素 | 一个输入区间 |
| `copy_if` | 复制满足条件的元素 | 一个输入区间与判断条件 |
| `transform` | 将元素转换后写入结果 | 一个或两个输入区间与转换函数 |

本节以 **C++11** 为兼容基准，介绍普通迭代器版本。输入区间均为左闭右开区间 `[first, last)`，不要求有序。

示例使用以下头文件。除 API 声明外，后续代码片段可分别放入 `main()` 中运行：

```cpp
#include <algorithm>  // copy、copy_if、transform
#include <iterator>   // back_inserter、back_insert_iterator
#include <vector>     // vector 动态数组
#include <string>     // string 字符串
#include <cctype>     // tolower 字符转换
using namespace std; // 竞赛示例中省略 std:: 前缀
```

---

# 一、copy：区间复制

## API

```cpp
// InputIt：输入迭代器类型；OutputIt：输出迭代器类型。
// 返回值：完成复制后的输出迭代器。
template<class InputIt, class OutputIt>
OutputIt copy(
    InputIt first,     // 输入起点，包含该位置
    InputIt last,      // 输入终点，不包含该位置
    OutputIt d_first  // 输出起点，必须能够连续写入全部结果
);
```

作用：将 `[first, last)` 中的元素按原有顺序复制到输出位置。

设输入长度为 $n$，则写入 $n$ 个元素。若输出的是普通容器迭代器，返回值指向**最后一个写入元素的后一位置**，不一定是整个容器的 `end()`。

## 示例：复制到已有空间

```cpp
// 示例 1：目标容器较大时，返回值表示本次写入的结束位置。
vector<int> a = {1, 2, 3, 4};
vector<int> b(6, -1);                  // 已存在 6 个可写元素

auto out = copy(a.begin(), a.end(), b.begin());
// b = {1, 2, 3, 4, -1, -1}；a 保持不变。
// out == b.begin() + 4，只有 [b.begin(), out) 是本次复制的结果。
```

对于这里的 `vector`，可以理解为依次执行 `b[i] = a[i]`。算法不会因为传入 `b.begin()` 就自动增加 `b.size()`。

---

# 二、目标空间与 back_inserter

## 1. reserve 不能代替 resize

直接通过普通迭代器输出时，目标元素必须已经存在。

```cpp
// 示例 2：reserve 只预留容量，resize 才会增加实际元素数量。
vector<int> a = {1, 2, 3, 4};
vector<int> b;
b.reserve(a.size());                 // capacity 足够，但 size 仍为 0

// 错误示范，保持注释，不要执行：b.begin() 没有可写元素。
// copy(a.begin(), a.end(), b.begin());

b.resize(a.size());                  // 现在存在 4 个可写元素
copy(a.begin(), a.end(), b.begin());  // 正确：b = {1, 2, 3, 4}
```

这里输入非空，错误调用会越过目标的有效元素范围，属于未定义行为。

## 2. back_inserter：将写入变成尾部插入

如果希望结果自动追加到容器末尾，可以使用 `back_inserter`。

```cpp
// 返回尾部插入迭代器；向它赋值时，会调用容器的 push_back。
// Container 必须支持 push_back，例如 vector、deque、string。
template<class Container>
back_insert_iterator<Container> back_inserter(
    Container& c  // 接收追加元素的容器
);
```

```cpp
// 示例 3：back_inserter 追加元素，不覆盖目标容器的已有内容。
vector<int> a = {1, 2, 3};
vector<int> b = {9};
b.reserve(b.size() + a.size());       // 可选：减少追加时的重新分配

copy(a.begin(), a.end(), back_inserter(b));
// b = {9, 1, 2, 3}；输入 a 与输出 b 是不同容器。
```

可以理解为：读取一个元素，就对目标容器执行一次 `push_back`。`set` 没有 `push_back`，因此不能使用 `back_inserter`。

| 输出方式 | 操作含义 | 是否自动增加 size |
| --- | --- | --- |
| `b.begin()` | 从已有元素开始覆盖 | 否 |
| `back_inserter(b)` | 从容器末尾追加 | 是 |

算法返回的类型与传入的输出迭代器类型对应。传入 `back_inserter(b)` 时，返回的是插入迭代器，**不能把它当成 `vector<int>::iterator`，也不能与 `b.begin()` 相减求数量**。

接口与插入迭代器定义可参见 [N3337：back_insert_iterator](https://timsong-cpp.github.io/cppwp/n3337/back.insert.iterator)。

---

# 三、copy_if：按条件复制

## API

```cpp
// UnaryPredicate：一元谓词，即根据一个元素判断条件是否成立的函数。
// 返回值：完成筛选复制后的输出迭代器。
template<class InputIt, class OutputIt, class UnaryPredicate>
OutputIt copy_if(
    InputIt first,     // 输入起点
    InputIt last,      // 输入终点，不包含
    OutputIt d_first, // 输出起点
    UnaryPredicate pred // pred(x) 为 true 时复制 x；不要修改输入元素
);
```

作用：只复制满足 `pred` 的元素，并保持这些元素在输入中的**相对顺序**。

## 示例：筛选偶数

```cpp
// 示例 4：使用 lambda 判断奇偶，使用 back_inserter 接收数量未知的结果。
vector<int> a = {5, 2, 4, 1, 6, 3};
vector<int> b;

copy_if(
    a.begin(), a.end(),
    back_inserter(b),
    [](int x) { return x % 2 == 0; }  // 一元谓词：偶数返回 true
);
// b = {2, 4, 6}；a 的元素、顺序和大小均不变。
```

如果没有元素满足条件，输出区间为空；使用尾部插入迭代器时，目标容器保持原状。

## 提前分配空间时，怎样得到实际结果？

假设输入长度为 $n$，最多会复制 $n$ 个元素，因此可以先分配 $n$ 个位置，再利用返回迭代器缩小容器。

```cpp
// 示例 5：copy_if 不会自动缩小已经分配好的目标 vector。
vector<int> a = {1, 2, 3, 4, 5, 6};
vector<int> b(a.size());              // 先创建最多可能需要的 6 个元素

auto out = copy_if(
    a.begin(), a.end(), b.begin(),
    [](int x) { return x % 2 == 0; }  // 只复制偶数
);
// 此时 b.size() 仍为 6；本次写入的有效结果为 [b.begin(), out)。

b.erase(out, b.end());               // 删除未使用的尾部，b = {2, 4, 6}
```

`copy_if` 的谓词调用次数由输入长度决定，即使没有任何元素被选中，也需要检查全部输入。保序与复制语义参见 [N3337：Copy](https://timsong-cpp.github.io/cppwp/n3337/alg.copy)。

---

# 四、transform 一元版本：一个元素映射为一个结果

## API

```cpp
// UnaryOperation：一元转换函数，接收一个输入元素并返回转换结果。
// 返回值：完成转换后的输出迭代器。
template<class InputIt, class OutputIt, class UnaryOperation>
OutputIt transform(
    InputIt first,     // 输入起点
    InputIt last,      // 输入终点，不包含
    OutputIt d_first, // 输出起点，可以等于 first 以进行原地转换
    UnaryOperation op // 返回结果交给算法写入；函数本身不要修改区间元素
);
```

作用：将每个输入元素交给 `op`，再把它的返回值写入对应的输出位置。

对于数组，可以按数据对应关系理解为：$b_i = f(a_i)$。

与 `copy_if` 不同，`transform` **不做筛选**：输入 $n$ 个元素，就输出 $n$ 个结果。

## 1. 转换到另一个容器

```cpp
// 示例 6：将 int 输入映射为 long long 平方值。
vector<int> a = {1, 2, 3, 4};
vector<long long> b(a.size());       // 输出类型可以与输入类型不同

auto out = transform(
    a.begin(), a.end(), b.begin(),
    [](int x) { return 1LL * x * x; } // 先转为 long long，再进行乘法
);
// b = {1, 4, 9, 16}；out == b.end()；a 保持不变。
```

转换函数的返回值必须能够赋给输出元素。输出容器类型更宽，并不代表函数内部的计算会自动使用更宽的类型，因此这里显式写成 `1LL * x * x`。

## 2. 原地转换

```cpp
// 示例 7：输出起点与输入起点相同，转换结果覆盖对应的原元素。
vector<int> a = {1, 2, 3, 4};

transform(
    a.begin(), a.end(), a.begin(),
    [](int x) { return x * 2; }       // 示例值较小，不会发生 int 溢出
);
// a = {2, 4, 6, 8}；元素个数保持不变。
```

这里由算法负责写回结果。lambda 只计算并返回新值，不应通过捕获引用、修改其他位置等方式干扰输入区间。

---

# 五、transform 二元版本：两个区间逐项配对

## API

```cpp
// BinaryOperation：二元转换函数，接收两个对应位置的输入元素。
// 返回值：完成转换后的输出迭代器。
template<class InputIt1, class InputIt2,
         class OutputIt, class BinaryOperation>
OutputIt transform(
    InputIt1 first1,    // 第一个输入区间的起点
    InputIt1 last1,     // 第一个输入区间的终点，不包含
    InputIt2 first2,    // 第二个输入区间的起点；此接口没有 last2
    OutputIt d_first,  // 输出起点，也可与 first1 或 first2 相同
    BinaryOperation op // op(x, y) 的返回值写入对应输出位置
);
```

作用：两个区间按位置配对，计算 $c_i = f(a_i, b_i)$。

**处理数量由第一个输入区间决定。** 若其长度为 $n$，第二个输入从 `first2` 开始必须至少有 $n$ 个可读元素，输出也必须能接收 $n$ 个结果。

它不会检查第二个区间长度，也不会自动取两个容器长度的较小值。

## 示例：对应位置相加

```cpp
// 示例 8：二元 lambda 的两个参数分别来自 a 和 b 的对应位置。
vector<int> a = {1, 2, 3};
vector<int> b = {10, 20, 30};
vector<int> c(a.size());             // 创建 3 个可写元素

auto out = transform(
    a.begin(), a.end(),
    b.begin(),
    c.begin(),
    [](int x, int y) { return x + y; } // 示例数据的和在 int 范围内
);
// c = {11, 22, 33}；out == c.end()。
```

也可以把输出参数改成 `back_inserter(c)`，但此时应根据需求让 `c` 初始为空；若已经创建了 $n$ 个元素，追加后会变成 $2n$ 个元素。

## 长度不同时怎么办？

先明确题目含义：如果要求完整逐项计算，应先检查长度；如果明确只处理公共长度，可以主动截取第一个区间。

```cpp
// 示例 9：需求明确为只计算两个 vector 的公共长度。
vector<int> a = {1, 2, 3, 4};
vector<int> b = {10, 20};
const auto n = min(a.size(), b.size()); // 主动确定安全的处理长度
vector<int> c(n);

transform(
    a.begin(), a.begin() + n,         // 只读取 a 的前 n 个元素
    b.begin(), c.begin(),
    [](int x, int y) { return x + y; } // 按位置相加
);
// c = {11, 22}；a 的剩余元素不参与本次转换。
```

一元、二元 API 和原地转换的允许情形参见 [N3337：Transform](https://timsong-cpp.github.io/cppwp/n3337/alg.transform)。该资料是 C++11 之后的早期工作草案，本节只使用其中的 C++11 接口。

---

# 六、常见应用：字符与数组映射

## 1. 英文字母转小写

```cpp
// 示例 10：按当前 C 字符分类规则转换；这里使用基本英文字符。
string s = "AbC123";

transform(
    s.begin(), s.end(), s.begin(),
    [](unsigned char ch) {           // 避免负 char 直接传入 tolower
        return static_cast<char>(std::tolower(ch));
    }
);
// s = "abc123"；数字字符保持不变。
```

`std::tolower` 接收的值必须能表示为 `unsigned char`，或者是 `EOF`。普通 `char` 可能为有符号类型，直接传入负值会违反这一要求，因此 lambda 参数使用 `unsigned char`。

这属于单字节字符处理，不是通用 Unicode 大小写转换。接口见 [N3337：C 字符串与字符处理](https://timsong-cpp.github.io/cppwp/n3337/c.strings)，参数取值要求见 [C 标准草案 N1570 §7.4](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf#page=218)。

## 2. 数字字符串转整数数组

```cpp
// 示例 11：前提是每个字符都属于 '0' 到 '9'。
string s = "5072";
vector<int> digits;

transform(
    s.begin(), s.end(), back_inserter(digits),
    [](char ch) { return ch - '0'; }  // 按单个字符转换，不是解析整个整数
);
// digits = {5, 0, 7, 2}。
```

类似地，平方、单位换算、字符编码映射、两组数据逐项合成都可以考虑 `transform`。如果循环包含复杂分支、提前退出或前后状态依赖，直接写循环通常更清楚。

---

# 七、容易出错的地方

## 1. 返回的是输出迭代器，不是输出容器

这三个算法都把结果写到传入的输出位置，不会返回一个新的 `vector`。

使用普通输出迭代器时，返回值表示本次写入的结束位置；使用插入迭代器时，返回值仍是插入迭代器。实际输出数量未知时，常用空容器配合 `back_inserter`，再读取容器的 `size()`。

## 2. 输入与输出重叠需要区分算法

| 情形 | 使用要求 |
| --- | --- |
| `copy` | 输出起点不能落在非空输入区间 `[first, last)` 内；某些向左覆盖复制合法，不应笼统记成“任何重叠都禁止” |
| `copy_if` | 使用互不重叠的输入、输出区间，不用它做原地筛选 |
| 一元 `transform` | 允许输出起点等于输入起点，逐项原地转换 |
| 二元 `transform` | 允许输出起点等于任一输入起点，但仍须保证读取和写入长度有效 |

初学时，复制与筛选优先使用独立目标容器；原地映射使用相同起点的 `transform`。不要用错位重叠的 `transform` 模拟整体平移，因为写入可能影响后续读取，而且转换函数的调用次序没有从左到右的保证。

不要一边遍历某个 `vector`，一边使用 `back_inserter` 向同一个 `vector` 追加。重新分配会使输入迭代器失效，即使没有重新分配，追加也会使旧的尾后迭代器失效。

## 3. 转换函数不要依赖调用顺序

`transform` 保证每个输入位置与对应输出位置的映射关系，但不要用捕获的计数器递增来假定函数按下标顺序调用。生成连续编号使用 `iota`；需要依次累积前面结果时，应使用合适的累积算法或显式循环。

## 4. 与 remove_if 区分

| 需求 | 常见做法 |
| --- | --- |
| 保留原容器，另存满足条件的元素 | `copy_if`，谓词为 true 时复制 |
| 从原容器中删除满足条件的元素 | `remove_if` 配合容器 `erase`，谓词为 true 时移除 |
| 元素数量不变，每个值按规则转换 | `transform` |

注意两个 `_if` 的判断方向不同：`copy_if` 的 true 表示“选中”，`remove_if` 的 true 表示“移除”。

---

# 八、复杂度总结

设输入长度为 $n$，其中有 $m$ 个元素满足筛选条件。二元 `transform` 的 $n$ 指第一个输入区间的长度。

| 算法 | 主要工作量 | 时间复杂度 | 输出元素数 |
| --- | --- | --- | --- |
| `copy` | $n$ 次赋值 | $O(n)$ | $n$ |
| `copy_if` | $n$ 次判断，复制 $m$ 个元素 | $O(n)$ | $m$ |
| 一元 `transform` | $n$ 次一元函数调用，并写入结果 | $O(n)$ | $n$ |
| 二元 `transform` | $n$ 次二元函数调用，并写入结果 | $O(n)$ | $n$ |

上述时间复杂度假设元素赋值、谓词和转换函数都是常数时间操作；若函数本身更复杂，需要把它的开销计入。对 `vector` 使用 `back_inserter` 时，尾部插入按均摊复杂度分析。

常规顺序实现只需少量迭代器，辅助空间通常为 $O(1)$，不包含用户函数的额外开销和输出存储。新建结果容器时，`copy / transform` 的结果占 $O(n)$ 空间，`copy_if` 的实际结果占 $O(m)$；若预分配了 $n$ 个位置或预留了 $n$ 的容量，其存储开销仍按 $O(n)$ 计。

---

# 九、本节总结

| 需求 | 优先考虑 | 核心参数 |
| --- | --- | --- |
| 原样复制一个区间 | `copy` | 输入区间、输出起点 |
| 复制满足条件的元素 | `copy_if` | 再加一个谓词 |
| 每个元素映射为一个新值 | 一元 `transform` | 再加一个一元转换函数 |
| 两个区间逐项计算 | 二元 `transform` | 第二个输入起点、二元转换函数 |
| 自动向结果末尾追加 | `back_inserter` | 接收结果的容器 |

重点掌握：

- 普通输出迭代器需要已有可写元素，`reserve` 不能替代 `resize`。
- `back_inserter` 负责追加，不会清空或覆盖已有内容。
- `copy_if` 保序筛选，`transform` 逐项映射。
- 二元 `transform` 没有第二个终点参数，必须自行保证长度。
- 写代码前先明确输入区间、输出位置、结果数量及是否允许原地处理。

本节属于**区间数据处理工具**，重点是把简单的复制、筛选与映射需求准确地表达为标准库调用。
