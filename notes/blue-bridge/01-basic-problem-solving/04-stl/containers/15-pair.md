# 1.4.15 `pair`：二元组与多关键字排序

## 1. 核心定位

`pair<T1, T2>` 将两个值绑定成一个对象，两个成员可以具有不同类型。它不是容器，而是 C++ 标准库中的**工具类型**，常用于坐标、区间端点、价格与质量、键值对，以及需要按多个关键字比较的数据。

```cpp
using pii = pair<int, int>;
using pll = pair<long long, long long>;

pii p = {3, 8};
cout << p.first << ' ' << p.second << '\n';  // 3 8
p.second = 10;                               // 两个成员都可以修改

pair<string, int> score = {"Alice", 95};
```

**最值得记住的性质**：`pair` 自带**字典序比较**。默认先比较 `first`，相等时才比较 `second`，因此可以直接配合 `sort()` 完成多关键字排序。

## 2. 常用接口、成员与类型

以 `pair<int, int>` 为例，以下列出 **C++11 常用形式**。构造函数本身没有返回类型；`first`、`second` 是数据成员，不是函数。

| 写法 / 签名 | 返回类型 / 效果 | 复杂度 |
|---|---|---|
| `pair<int, int> p(a, b)` | 构造对象 `p`，分别用 `a`、`b` 初始化两个成员 | `O(1)` |
| `pair<int, int> p{a, b}` | C++11 列表初始化，效果同上 | `O(1)` |
| `make_pair(a, b)` | 根据参数推导类型；若 `a`、`b` 为 `int`，返回 `pair<int, int>` | `O(1)` |
| `p.first` / `p.second` | 对于非 `const` 的 `p`，均为可修改的 `int` 成员 | `O(1)` |
| `get<0>(p)` / `get<1>(p)` | 对非 `const` 左值 `p` 返回 `int&` | `O(1)` |
| `bool operator==(const pair<int, int>& x, const pair<int, int>& y)` | 两个成员分别相等时为 `true` | `O(1)` |
| `bool operator<(const pair<int, int>& x, const pair<int, int>& y)` | 按字典序比较两个二元组 | `O(1)` |
| `void swap(pair<int, int>& other)`（成员函数） | 交换两个二元组的成员值 | `O(1)` |

表中比较和构造复杂度以两个成员均为整数为前提；如果成员是字符串等对象，还需计入成员自身的构造、比较及交换成本。

比较运算符实际以函数模板形式提供，这里写出作用于 `pair<int, int>` 时的参数与返回类型。

### 2.1 字典序：先比较第一关键字，再比较第二关键字

```cpp
pair<int, int> a = {2, 100};
pair<int, int> b = {3, 1};
pair<int, int> c = {2, 101};

cout << (a < b) << '\n';  // 1：只看第一关键字，2 < 3
cout << (a < c) << '\n';  // 1：第一关键字相等，再比较 100 < 101
```

等价的比较逻辑可以理解为：

```cpp
bool lessPair(const pii& a, const pii& b) {
    if (a.first != b.first) return a.first < b.first;
    return a.second < b.second;
}
```

因此，**默认升序排序就是“第一关键字升序，第一关键字相等时第二关键字升序”**：

```cpp
vector<pii> v = {{3, 8}, {1, 9}, {3, 2}, {1, 4}};
sort(v.begin(), v.end());
// (1, 4), (1, 9), (3, 2), (3, 8)
```

### 2.2 与 `vector`、`priority_queue` 配合

```cpp
vector<pii> a;
a.emplace_back(5, 10); // 直接构造 pair(5, 10)
a.push_back({2, 7});  // 插入已经构造好的 pair 值
```

`vector<pii>::emplace_back(Args&&... args)` 在 **C++11 中返回 `void`**，尾部插入均摊 `O(1)`，单次扩容时可能为 `O(n)`。

```cpp
priority_queue<pii> mx; // 默认最大堆：先比较 first，平手比较 second
mx.push({3, 1});
mx.push({3, 8});
cout << mx.top().second; // 8

priority_queue<pii, vector<pii>, greater<pii>> mn; // 字典序最小堆
```

**注意**：默认排序会用第二关键字打破平局。如果题目要求“第二关键字降序”或其他规则，需要自定义比较器，不能直接使用默认字典序。

## 3. 代表题：[CF 456A — Laptops](https://codeforces.com/problemset/problem/456/A)

**题型**：二元组排序 / 二维条件判定 / 单调性 / 利用传递性消除全局枚举。

每台笔记本电脑有价格 `price` 和质量 `quality`。如果存在两台电脑，使得**价格更低的那台质量反而更高**，输出 `Happy Alex`；否则输出 `Poor Alex`。

即判断是否存在两台电脑 `i`、`j` 满足：

\[
price_i<price_j,\qquad quality_i>quality_j.
\]

原题保证价格互不相同。直接枚举两台电脑需要 `O(n²)`，但可以利用排序与单调性降为 `O(n log n)`。

### 3.1 第一步：排序固定一个比较维度

每台电脑用 `{price, quality}` 表示：

```cpp
vector<pii> a(n);
for (int i = 0; i < n; ++i) {
    cin >> a[i].first >> a[i].second;
}
sort(a.begin(), a.end());
```

因为 `pair` 先比较 `first`，排序后价格从低到高排列。又由于本题的价格互不相同，有：

\[
price_1<price_2<\cdots<price_n.
\]

于是，任意两个排序后的位置 `i < j`，**价格关系已经确定**；只需判断质量是否发生逆序：

\[
quality_i>quality_j.
\]

这一步将原本包含两个严格不等式的二维判断，转化为一维质量序列的性质判断。

### 3.2 第二步：利用传递性，将任意两点比较化为相邻比较

例如排序后：

```text
价格：1   2   3   4   5
质量：2   4   7   3   6
```

质量在第三、四台之间从 `7` 降到 `3`，因此第三台**更便宜且质量更高**，输出 `Happy Alex`。

更深一层的问题是：**如果违背关系的两台电脑并不相邻，会不会漏判？**

不会。考虑其逆否关系：如果每对相邻位置都满足

\[
quality_i\le quality_{i+1},
\]

由大小关系的**传递性**，必然得到

\[
quality_1\le quality_2\le\cdots\le quality_n.
\]

所以任意 `i < j` 都满足 `quality_i <= quality_j`，不存在“更便宜却质量更高”的一对，输出 `Poor Alex`。

反过来，**只要质量序列不是非递减的，就必然存在一处相邻下降**；该相邻位置本身就构成符合条件的一对，输出 `Happy Alex`。

> **排序固定一个维度 → 将全局关系转化为单调性 → 利用传递性，只检查相邻元素。**

### 3.3 C++11 实现

```cpp
void solve() {
    int n;
    cin >> n;

    vector<pii> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i].first >> a[i].second;
    }

    // pair 按 first（价格）优先、second（质量）其次的字典序排序
    sort(a.begin(), a.end());

    for (int i = 1; i < n; ++i) {
        // 原题价格互不相同；相邻位置的价格严格递增
        // 因而一旦质量下降，立刻找到便宜却更好的电脑
        if (a[i - 1].first < a[i].first &&
            a[i - 1].second > a[i].second) {
            cout << "Happy Alex\n";
            return;
        }
    }

    cout << "Poor Alex\n";
}
```

**复杂度**：读取与扫描 `O(n)`，排序 `O(n log n)`，总时间 `O(n log n)`；存储电脑信息需要 `O(n)` 空间。

### 3.4 为什么可以省略价格判断？

上面的两个严格不等式直接对应题意，表达清楚，可以保留。但对于本题，排序后相邻价格必然严格递增，因此只检查质量即可：

```cpp
if (a[i - 1].second > a[i].second) {
    cout << "Happy Alex\n";
    return;
}
```

甚至考虑一般的**价格可能相同**的输入，`pair` 的字典序仍能确保：相同价格的一组元素内部按质量非递减排列，所以**相邻质量下降不可能发生在价格相同的两台之间**。因此，发生下降时同样必然满足价格严格上升。

这个简化依赖于**默认的 `pair` 字典序排序**；如果排序改成只比较价格，且可能出现同价电脑，就不能不加分析地省略价格条件。

## 4. 易错点与选型

| 易错点 | 原因 / 正确做法 |
|---|---|
| 认为 `sort(vector<pair<...>>)` 只比较 `first` | `first` 相同时会继续比较 `second` |
| 把 `pair` 比较理解成两个成员同时更小 | 实际是字典序比较，并非逐分量大小比较 |
| 需要第二关键字降序却使用默认 `sort` | 必须按题意设计自定义比较函数 |
| 认为 `vector::emplace_back()` 在 C++11 返回新元素引用 | C++11 返回 `void`；C++17 起才返回引用 |
| 数据超过两个成员仍不断嵌套 `pair` | 尽量使用命名清楚的 `struct`，提高可读性 |
| 对 CF 456A 直接进行两层枚举 | 先排序，再用传递性将全局单调性判断缩减为相邻扫描 |

## 5. 选型触发器

> **两个属性天然绑定、需要一起移动或排序 → `pair`。如果按第一、第二关键字依次升序，直接利用字典序；如果规则不同，自定义比较器。**

CF 456A 更进一步说明：**排序并不只是让数据变有序，而是固定某个维度的相对关系，使原本需要全局比较的问题变成可通过局部单调性判断的问题。**
