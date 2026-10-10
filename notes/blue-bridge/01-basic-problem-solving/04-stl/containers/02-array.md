# 1.4.2 `array`：固定长度顺序容器

## 1. 核心定位

`std::array<T, N>` 是**大小在编译期确定**的顺序容器，元素连续存储，支持随机访问，并具有与其他 STL 容器一致的迭代器接口。

```cpp
#include <array>
using namespace std;

array<int, 5> a{};           // 5 个 int，全部初始化为 0
array<int, 3> b = {1, 2, 3};
array<bool, 26> seen{};      // 26 个 false

// N 必须是编译期常量
constexpr int N = 5;
array<int, N> c{};
```

典型用途：**字母 / 数字频次统计、固定数量的状态、方向数组和小规模定长数据**。

如果长度需要根据运行时输入决定，通常选择 `vector`。

---

## 2. 常用 API

下表以 `array<int, N>` 的**非 const 对象**为例，使用标准库中的类型别名 `size_type`、`iterator`；仅列竞赛常用重载。

| API 签名（常用形式） | 作用与返回值 | 复杂度 |
|---|---|---|
| `int& operator[](size_type pos)` | 返回下标 `pos` 处元素的引用；**不检查越界** | \(O(1)\) |
| `int& at(size_type pos)` | 返回元素引用；越界抛出 `out_of_range` | \(O(1)\) |
| `int& front()` | 返回首元素引用 | \(O(1)\) |
| `int& back()` | 返回末元素引用 | \(O(1)\) |
| `constexpr size_type size() const noexcept` | 返回固定元素数量 `N` | \(O(1)\) |
| `constexpr bool empty() const noexcept` | 判断是否为空（即 `N == 0`） | \(O(1)\) |
| `void fill(const int& value)` | 将全部元素赋值为 `value` | \(O(N)\) |
| `iterator begin() noexcept` | 返回首元素迭代器 | \(O(1)\) |
| `iterator end() noexcept` | 返回尾后迭代器 | \(O(1)\) |

`const array` 的访问接口返回 `const int&` 或 `const_iterator`。实际使用通常借助 `auto`，不必死记这些类型别名。

```cpp
array<int, 5> a{};
a.fill(7);               // {7, 7, 7, 7, 7}
a[2] = 10;

cout << a.front();       // 7
cout << a.back();        // 7
cout << a.size();        // 5

sort(a.begin(), a.end()); // 可以直接配合 STL 算法
```

**没有** `push_back()`、`pop_back()`、`resize()`：`array` 不能改变长度。

---

## 3. 高频陷阱

### 3.1 `{}` 与 `()`：零初始化还是函数声明？

```cpp
array<int, 26> a;     // 局部变量：int 元素未初始化
array<int, 26> b{};   // 26 个元素全部为 0（推荐）
array<int, 26> c();   // 不是数组！声明了返回 array<int,26> 的函数 c
```

最后一种属于 C++ 的 **Most Vexing Parse**：语法能解析为函数声明时，会按函数声明处理。

```cpp
array<int, 26> a{};  // 计数或标记数组的稳妥起点
// 或者：
array<int, 26> b;
b.fill(0);
```

`fill(0)` 会对元素赋值；`{}` 则在定义时直接完成初始化。

### 3.2 长度必须是编译期常量

```cpp
int n;
cin >> n;

// array<int, n> a; // 错：n 是运行时变量
vector<int> a(n);   // 正确
```

### 3.3 字符映射：先判断范围，再访问数组

```cpp
array<int, 26> seen{};

for (char c : s) {
    if (c >= 'a' && c <= 'z') {
        seen[c - 'a'] = 1;
    }
}
```

`isalpha(c)` 还可能接受大写字母；它**不等于** `c` 一定在 `'a'` 到 `'z'` 之间。使用 `c - 'a'` 作为下标时，应检查与下标映射对应的字符范围。

### 3.4 复制的是元素，不是共享同一数组

```cpp
array<int, 3> a = {1, 2, 3};
array<int, 3> b = a;  // 逐元素复制，O(N)
b[0] = 100;

cout << a[0];         // 1
```

这里对 `int` 的复制会得到互不影响的值；如果元素是指针，则复制的是指针本身，指向的对象并不会自动复制。

### 3.5 `array<T, 0>` 不可访问首尾元素

`array<int, 0>` 合法，但它没有可访问的元素，不能使用 `front()`、`back()` 或 `a[0]`。

---

## 4. 代表题：[CF 443A — Anton and Letters](https://codeforces.com/problemset/problem/443/A)

**题型：字符去重 / 固定值域标记**

题意：给定形如 `{a, b, a, c}` 的字符集合描述，求出现了多少种不同的小写英文字母。

### 思路与独立实现

由于值域只有 26 个小写字母，不需要排序或哈希表；直接用定长数组记录是否出现。

```cpp
void solve() {
    string s;
    getline(cin, s);

    array<int, 26> a;
    a.fill(0);

    for (char c : s) {
        int i = c - 'a';
        if (isalpha(c)) {
            a[i] = 1;
        }
    }

    int ans = 0;
    for (int v : a) ans += v;

    cout << ans << '\n';
}
```

原解在题目保证仅有小写字母的前提下正确；`getline` 能完整读入包含空格、逗号与花括号的整行输入。

### 推荐修改（只展示差异）

**零初始化**可以直接写：

```cpp
array<int, 26> a{};
```

**字符判断与下标计算**应保持一致：

```cpp
for (char c : s) {
    if (c >= 'a' && c <= 'z') {
        a[c - 'a'] = 1;
    }
}
```

其余代码保持原样即可，无须重复整份程序。

复杂度：若输入字符串长度为 \(L\)，遍历输入为 \(O(L)\)，累加 26 个标记为 \(O(26)\)；总时间 \(O(L+26)\)，额外空间 \(O(26)\)。

---

## 5. 选型触发器

> **值域小且固定 / 状态个数编译期已知 → 优先考虑 `array` 或普通定长数组。**

常见场景：

```cpp
array<int, 26> freq{};   // 26 个字母的频率
array<int, 10> cnt{};    // 0~9 数字计数
array<int, 4> dx = {-1, 0, 1, 0}; // 固定方向
```

- 需要动态改变元素个数：用 `vector`。
- 需要随机访问、固定长度、统一 STL 接口：可选 `array`。
- `array<int, N> a();` **是函数声明**；创建并清零请写 `array<int, N> a{};`。
