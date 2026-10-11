# 1.4 Algorithms · 01 `sort` 与 `stable_sort`：排序规则与稳定性

## 1. 核心定位与选型

`sort()` 和 `stable_sort()` 都对**左闭右开区间 `[first, last)` 原地排序**，返回 `void`。最重要的区别是：

- **`sort`**：通常首选，保证 `O(n log n)` 次比较，**不保证稳定性**。
- **`stable_sort`**：当比较器认为两个元素**等价**时，仍保留它们在排序前的相对顺序；通常需要额外辅助空间。

```cpp
vector<int> a = {3, 1, 4, 2};

sort(a.begin(), a.end());                 // 1 2 3 4
sort(a.begin(), a.end(), greater<int>()); // 4 3 2 1
sort(a.rbegin(), a.rend());               // 再次按升序排列（逆向区间默认升序）
```

**选型原则**：普通排序优先 `sort`；只有**相同排序优先级的元素必须保持原先顺序**时，才需要 `stable_sort`。如果平局有明确的第二、第三关键字，应直接写入比较规则，而不是误以为稳定排序会自动处理。

## 2. 常用 API：签名、返回类型与复杂度

以下为 C++11 常用函数模板签名，`RandomIt` 表示随机访问迭代器，`Compare` 表示比较器类型。

| API 签名 | 返回类型 / 作用 | 比较次数 |
|---|---|---|
| `template<class RandomIt> void sort(RandomIt first, RandomIt last)` | `void`；按默认 `<` 升序排列 | `O(n log n)` |
| `template<class RandomIt, class Compare> void sort(RandomIt first, RandomIt last, Compare comp)` | `void`；按指定比较器排列 | `O(n log n)` |
| `template<class RandomIt> void stable_sort(RandomIt first, RandomIt last)` | `void`；稳定的默认升序排列 | 辅助内存充足时 `O(n log n)`，否则可达 `O(n log² n)` |
| `template<class RandomIt, class Compare> void stable_sort(RandomIt first, RandomIt last, Compare comp)` | `void`；按指定比较器稳定排序 | 辅助内存充足时 `O(n log n)`，否则可达 `O(n log² n)` |

这里 `n = last - first`，复杂度指**比较次数**；如果比较字符串或复杂对象，还要计入单次比较和元素移动的成本。

- `vector`、`array`、`deque` 的迭代器支持随机访问，可以使用上述算法。
- **`set`、`map` 的迭代器不支持随机访问**，不能对其调用 `std::sort`；它们本身已维护有序性。
- **`list` 也不支持 `std::sort`**，应使用其成员函数 `list::sort()`。

### 2.1 为什么 `sort` 和 `stable_sort` 的实现不同？

C++ 标准规定的是**行为与复杂度**，没有强制某一种底层排序算法。常见实现思路如下：

> **`sort`：内省排序（Introsort）**。以快速排序的划分思想提高常见情况的效率；当递归层数过深时切换堆排序，避免纯快排可能出现的 `O(n²)` 最坏情况；小区间常用插入排序减少常数开销。因此通常既快，又能满足最坏 `O(n log n)` 次比较的要求。

> **`stable_sort`：稳定归并排序一类的算法**。合并两个有序段时，必须保证相同关键字的元素仍按原先顺序出现。借助临时缓冲区，左右两段可以线性合并，因此通常需要 `O(n)` 额外空间，并能完成 `O(n log n)` 次比较。

如果没有足够的辅助缓冲区，实现可以采用**更复杂的原地稳定合并**：普通缓冲区归并在每一层合并的总成本为 `O(n)`，而无足够缓冲区时，每层的稳定合并比较成本可能升至 `O(n log n)`；递归划分有 `O(log n)` 层，所以总比较次数可能为：

\[
O(n\log n)\times O(\log n)=O(n\log^2 n).
\]

这解释了 `stable_sort` 的 `log² n` 来源：**归并层数的一个 `log n`，与更昂贵的无缓冲稳定合并产生的另一个 `log n`**。这是复杂度上界的来源，不代表每次调用都会这么慢。

### 2.2 稳定性由比较器定义，而非由整个对象是否相等定义

```cpp
vector<pair<int, int>> a = {
    {90, 3}, // {成绩, 学号}
    {80, 2},
    {90, 1}
};

stable_sort(a.begin(), a.end(), [](const pair<int, int>& x,
                                   const pair<int, int>& y) {
    return x.first > y.first; // 只按成绩降序
});
// (90, 3), (90, 1), (80, 2)
```

两个 `90` 分学生虽然学号不同，但比较器对它们两个方向都返回 `false`，因此视为**排序等价**，稳定排序保留二者原先的顺序。

如果改为默认的 `pair` 字典序比较，学号也会参与排序，二者就不再等价；`stable_sort` 也不会保留原顺序。

## 3. 什么时候可以直接排序？什么时候要自定义？

**先确认题目要求的关键字优先级与升降序方向，再决定是否需要比较器。**

| 数据与排序要求 | 直接写法 / 处理方式 | 是否需要自定义 |
|---|---|---|
| 整数、字符串默认升序 | `sort(v.begin(), v.end())` | 否 |
| 整数、字符串整体降序 | `sort(v.begin(), v.end(), greater<T>())` | 否 |
| `pair`、`tuple` **所有字段**按字典序升序 | `sort(v.begin(), v.end())` | 否 |
| `pair`、`tuple` **所有字段**按字典序降序 | `sort(v.begin(), v.end(), greater<T>())` | 否 |
| 第一关键字降序、第二关键字升序等混合规则 | Lambda / 命名比较函数，或在安全时转换关键字 | 通常需要额外处理 |
| `struct` 等自定义类型 | 定义 `operator<` 后默认排序；或给 `sort` 传入比较器 | 需要提供比较规则 |
| 比较器等价时必须保留先后顺序 | `stable_sort(..., comp)` | 是否需要 `comp` 取决于排序规则 |

其中 `T` 是元素的真实类型，例如 `int`、`pair<int, int>` 或 `tuple<int, int, int>`。

### 3.1 `pair`、`tuple` 默认比较已有字典序

```cpp
using pii = pair<int, int>;
vector<pii> v = {{2, 100}, {1, 5}, {2, 3}};

sort(v.begin(), v.end());                 // (1,5), (2,3), (2,100)
sort(v.begin(), v.end(), greater<pii>()); // (2,100), (2,3), (1,5)
```

以上的“降序”是**整体字典序降序**，不是“第一字段降序、第二字段升序”。`tuple` 也同理：先比较第一个字段，平手再依次比较后面的字段。

一种不额外传比较器的降序写法是 `sort(v.rbegin(), v.rend())`：在逆向迭代器区间上默认升序，最终从正向看就是降序。这对于**只定义了 `<` 的自定义类型**也很有用。

如果希望“解题数降序、罚时升序”，应该明确写成：

```cpp
sort(v.begin(), v.end(), [](const pii& a, const pii& b) -> bool {
    if (a.first != b.first) return a.first > b.first;
    return a.second < b.second;
});
```

也可以在数据范围允许时存储 `{-solved, penalty}`，用默认升序完成混合规则，但**取负前应排除 `INT_MIN` 等溢出情况**。

### 3.2 自定义 `struct`：`operator<` 的两种简洁写法（C++11）

C++11 的普通 `struct` **不会自动获得字段字典序比较**。要调用无比较器的 `sort(v.begin(), v.end())`，需要提供可用的 `<` 运算；也可以不定义 `<`，直接传入比较器。

```cpp
struct Point {
    int x, y;
};

// 写法 A：tie 复用 tuple 的字典序（需 #include <tuple>）
bool operator<(const Point& a, const Point& b) {
    return tie(a.x, a.y) < tie(b.x, b.y);
}
```

如果不想用 `tie()`，可以改为等价的手写版本（**两种写法二选一，不可同时重复定义**）：

```cpp
// 写法 B：手写关键字比较
bool operator<(const Point& a, const Point& b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}
```

之后可直接：

```cpp
vector<Point> points = {{2, 5}, {1, 9}, {2, 3}};
sort(points.begin(), points.end());  // (1,9), (2,3), (2,5)
```

注意：**仅定义 `<` 不代表定义了 `>`**。因此 `greater<Point>()` 在 C++11 中不一定能用；如需完全逆序，可改用 `sort(points.rbegin(), points.rend())`，或另写比较器。

如果只有一个地方需要特殊排序，也可以只传入 Lambda，不必为类型定义全局 `operator<`：

```cpp
sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
    return tie(a.y, a.x) < tie(b.y, b.x); // y 优先升序，平手按 x 升序
});
```

### 3.3 `greater<T>`：类型与对象不要混淆

```cpp
sort(v.begin(), v.end(), greater<pii>()); // 正确：传比较器对象
// sort(v.begin(), v.end(), greater<pii>); // 错误：这里只写了类型

priority_queue<pii, vector<pii>, greater<pii>> pq;
// priority_queue 的第三个模板参数需要的是比较器类型
```

- `greater<pii>` 是**类型**；`greater<pii>()` 或 `greater<pii>{}`（C++11）创建比较器**对象**。
- 普通命名函数 `bool cmp(const pii&, const pii&)` 可以直接以 `cmp` 作为 `sort` 的第三个实参；函数名可转为函数指针，不要写成缺少实参的 `cmp()`。

## 4. 什么是严格弱序（Strict Weak Ordering）？

`sort()` 和 `stable_sort()` 要求比较器提供一致的“排在前面”关系。设 `comp(a, b)` 为 `true` 表示 `a` 应排在 `b` 前面。

- **严格性（自反性被排除）**：`comp(a, a)` 必须是 `false`，不能用 `<=`、`>=` 把相等元素也判定为“在前”。
- **非对称性**：若 `comp(a, b)` 为 `true`，则 `comp(b, a)` 必须为 `false`。
- **传递性**：若 `a` 排在 `b` 前、`b` 排在 `c` 前，那么 `a` 也应排在 `c` 前，不能形成循环。
- **等价关系也要一致**：若两个方向的比较都为 `false`，它们属于同一排序优先级；这样的等价关系也必须满足传递性，才能合理分组。

> 因此“弱”不是说比较器可以随意写，而是允许**不同对象拥有相同排序优先级**：

```cpp
// 只看成绩：成绩相等的不同学生，被视为排序等价
bool cmp(const pii& a, const pii& b) {
    return a.first > b.first;
}
```

反例：

```cpp
// 错误：两个成绩相同的元素可能彼此都被判定为排在前面
return a.first >= b.first;
```

违反严格弱序是未定义行为，不能只依赖“某几个样例碰巧能排序正确”。

## 5. 代表题：[CF 166A — Rank List](https://codeforces.com/problemset/problem/166/A)

**题型**：自定义混合排序 / 并列排名 / 排序后相同元素连续 / 去除不必要的哈希表。

每支队伍用 `{solved, penalty}` 表示，排名要求：解题数**降序**；解题数相同时罚时**升序**。给定排名位置 `k`，输出与第 `k` 名**两个属性均相同**的队伍数量。

### 5.1 排序规则不能直接用 `greater<pii>()`

`greater<pii>()` 会把两个字段**都按字典序降序**，从而导致相同解题数时罚时较大的队伍排在前面，违背题意。应写成混合比较：

```cpp
bool cmp(const pii& a, const pii& b) {
    if (a.first != b.first) return a.first > b.first;
    return a.second < b.second;
}
```

此外，题目给出的名次从 `1` 开始，`vector` 下标从 `0` 开始：**第 `k` 名是 `a[k - 1]`**，不是 `a[k]`。

### 5.2 先想到哈希统计，再利用排序消除额外数据结构

一种思路是：用 `unordered_map<pii, int, PairHash>` 统计每种成绩出现的次数；排序得到第 `k` 名后，查询对应频率。但 C++11 的 `pair` 没有内置通用哈希，而且**排序已将相同成绩聚集为连续区间**，无需再专门维护哈希表。

取出 `a[k-1]` 后，从该位置向左右扩展相等元素即可。两个属性都相等时，`pair::operator==` 为 `true`。

### 5.3 C++11 完整实现

```cpp
using pii = pair<int, int>;

bool cmp(const pii& a, const pii& b) {
    if (a.first != b.first) return a.first > b.first;
    return a.second < b.second;
}

void solve() {
    int n, k;
    cin >> n >> k;

    vector<pii> a(n);
    for (int i = 0; i < n; ++i)
        cin >> a[i].first >> a[i].second;

    sort(a.begin(), a.end(), cmp);

    const pii target = a[k - 1];
    int l = k - 1, r = k - 1;

    // 相同成绩在排序后连续，向两侧扩展等值段
    while (l > 0 && a[l - 1] == target) --l;
    while (r + 1 < n && a[r + 1] == target) ++r;

    cout << r - l + 1 << '\n';
}
```

若只想让代码更短，也可以使用：

```cpp
cout << count(a.begin(), a.end(), a[k - 1]) << '\n';
```

但 `std::count` 会线性遍历整个区间，**并不利用等值元素已连续的性质**；它属于后续“查找与计数”模块的接口。

**复杂度**：排序 `O(n log n)`，向两侧扩展至多 `O(n)`，总时间 `O(n log n)`；保存队伍信息 `O(n)` 空间。相较复合键哈希的实现，这里不需要额外的哈希表或哈希函数。

> **排序除了确定先后顺序，还能把相同关键字聚到连续区间，使分组与频率统计直接转化为相邻扫描。**

## 6. 高频易错点与选型触发器

| 易错点 | 修正方式 |
|---|---|
| 需要混合升降序，却直接使用 `greater<pair<...>>()` | `greater` 对整个字典序取反，不会自动转换某一字段方向 |
| `sort(..., greater<T>)` | 需要比较器对象：`greater<T>()` 或 `greater<T>{}` |
| `return x <= y` / `return x >= y` | 违反严格弱序；应使用严格比较并处理平局 |
| `stable_sort` 自动处理题目的所有平局规则 | 它只保证比较器等价元素保留原始相对顺序 |
| 自定义 `struct` 未定义 `<`，却调用默认 `sort` | 自定义 `operator<` 或传 Lambda / 比较函数 |
| `sort(s.begin(), s.end())`，其中 `s` 为 `set` | `set` 已有序，迭代器也不支持随机访问 |
| 把第 `k` 名写成 `a[k]` | 容器从 `0` 开始编号，正确位置是 `a[k - 1]` |
| 排序后仍为相同元素维护哈希表 | 若允许离线处理，先考虑等值连续区间的直接扫描 |

**选型顺序**：先看默认字典序是否满足全部关键字规则 → 再决定是否自定义比较器或转换字段 → 最后判断是否需要稳定性。无需为普通排序强行选择 `stable_sort`。

**参考**：[std::sort](https://en.cppreference.com/w/cpp/algorithm/sort) · [std::stable_sort](https://en.cppreference.com/w/cpp/algorithm/stable_sort)
