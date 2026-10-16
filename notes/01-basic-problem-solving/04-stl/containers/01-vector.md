# 1.4.1 `vector`：动态顺序数组

## 1. 定位与选型

`vector` 使用连续内存存储元素，支持 **随机访问 O(1)**、**尾部追加均摊 O(1)**；在中间插入或删除通常需要移动元素，代价为 O(n)。

**适用场景：**动态收集元素、按下标访问、顺序遍历，以及预处理一个固定序列后反复查询。

```cpp
vector<int> a;             // 空 vector
vector<int> b(5);          // 5 个 0
vector<int> c(5, 7);       // 5 个 7
vector<int> d = {1, 2, 3};
```

## 2. 常用 API

以下以 `vector<int>` 为例。`size_type` 是容器的大小类型，`iterator` / `const_iterator` 是对应迭代器类型；表格列出常用签名，省略其他重载与部分限定符。

复杂度中的 `n` 表示调用前的元素数，`k` 表示输入区间或初始化列表的元素数。

| API 签名 | 作用 / 返回值 | 复杂度 |
|---|---|---|
| `void push_back(const int& value)` | 尾部追加一个元素 | **均摊 O(1)**，单次扩容可能 O(n) |
| `void pop_back()` | 删除末尾元素；**不返回值** | O(1) |
| `int& operator[](size_type pos)` | 通过下标访问元素；不检查越界 | O(1) |
| `int& front()` / `int& back()` | 返回首 / 尾元素的引用 | O(1) |
| `size_type size() const` | 当前元素个数 | O(1) |
| `bool empty() const` | 是否为空 | O(1) |
| `size_type capacity() const` | 当前可容纳的元素数 | O(1) |
| `void reserve(size_type new_cap)` | 将容量至少扩至 `new_cap`，**不改变元素个数** | 发生重分配时 O(n) |
| `void resize(size_type count)` | 将元素个数改为 `count`；新增的 `int` 为 0，保留部分不变 | 最坏 O(n + count) |
| `void resize(size_type count, const int& value)` | 改变元素个数；**仅新增元素**取 `value` | 最坏 O(n + count) |
| `void assign(size_type count, const int& value)` | 用 `count` 个 `value` **替换全部内容**；返回 `void` | O(n + count) |
| `template<class InputIt> void assign(InputIt first, InputIt last)` | 用 `[first, last)` 的元素替换全部内容 | O(n + k) |
| `void assign(initializer_list<int> values)` | 用初始化列表替换全部内容，C++11 支持 | O(n + k) |
| `iterator insert(const_iterator pos, const int& value)` | 在 `pos` 前插入，返回新元素位置 | O(n) |
| `iterator erase(const_iterator pos)` | 删除该位置，返回其后一个元素的位置 | O(n) |
| `void clear()` | 删除所有元素，保留的容量不保证缩小 | O(n) |
| `iterator begin()` / `iterator end()` | 首元素 / 尾后迭代器 | O(1) |

```cpp
vector<int> v = {10, 20, 30};

v.push_back(40);            // {10, 20, 30, 40}
v.pop_back();               // {10, 20, 30}
cout << v[1];               // 20
v.erase(v.begin() + 1);     // {10, 30}

for (int x : v) cout << x << ' ';
```

`front()`、`back()`、`pop_back()` 要求非空；`end()` 是尾后位置，不能解引用。

## 3. 关键机制与易错点

### 3.1 为什么 `push_back()` 是“均摊 O(1)”？

`size()` 是实际元素数，`capacity()` 是已分配容量。容量足够时，尾部追加为 O(1)；容量不足时，可能重新分配连续空间并搬运全部旧元素，**单次**代价为 O(n)。

以容量按倍数增长作为示意：

```text
1 → 2 → 4 → 8 → 16 → ...
```

连续插入 n 个元素时，扩容搬运的累计工作量为几何级数，总计 O(n)，因此 n 次尾部追加总计 O(n)，**均摊每次 O(1)**。实际扩容倍率由实现决定，并不保证翻倍。

> 均摊复杂度不是“每次都 O(1)”，也不是依靠随机概率得到的平均复杂度。

### 3.2 `reserve()` 不等于 `resize()`

```cpp
vector<int> a;
a.reserve(100);  // size() == 0，不能访问 a[0]

vector<int> b;
b.resize(100);   // size() == 100，b[0] == 0
```

- `reserve(n)`：只预留容量，不创建元素。
- `resize(n)`：改变元素数量；扩张时新增 `int` 初始化为 0。

**不能把 `capacity()` 当成合法下标范围；合法下标取决于 `size()`。**

### 3.3 迭代器、引用和下标

- `push_back()` 若引发重分配，原有迭代器、指针、引用全部失效；若未重分配，旧的 `end()` 仍会失效。
- `erase(pos)` 会使被删位置及之后的迭代器、引用失效。遍历时删除元素，应接住 `erase()` 返回的下一位置或谨慎控制下标。
- `v[i]` 不检查边界；只有 `0 <= i < v.size()` 才能正常访问。
- `vector<int> b = a;` 会复制元素，两份整数互不影响；若元素是指针，复制指针并不意味着复制其指向的对象。

### 3.4 `assign()`：替换全部内容，与其他操作对照

**`assign` 同时确定新的长度和全部元素值。** 它不是尾部追加，也不是只填充新增位置；旧内容不保留。

```cpp
vector<int> a = {10, 20, 30};
a.assign(5, -1); // 替换为 {-1, -1, -1, -1, -1}，size() == 5

vector<int> src = {2, 4, 6, 8};
a.assign(src.begin() + 1, src.end()); // 复制半开区间，结果为 {4, 6, 8}

a.assign({7, 9}); // 初始化列表形式，结果为 {7, 9}
a.assign(0, -1);  // 新长度为 0，结果为空
```

区间形式要求 `[first, last)` 有效，且来源迭代器不能属于正在被 `assign` 的这个 `vector`；从另一个容器复制更明确。调用后旧迭代器、引用和指针均不可继续依赖，应重新获取。

#### `assign`、`resize`、`reserve`、`reverse` 分别改变什么

下表各行都**独立从 `{10, 20, 30}` 开始**，不是顺序执行：

| 操作 | 操作后的 `size()` | 元素内容 | 容量行为 |
| --- | --- | --- | --- |
| `a.assign(5, -1)` | 5 | 全部替换为 5 个 `-1` | 可能重分配，不保证容量恰好等于 5 |
| `a.resize(5, -1)` | 5 | `{10, 20, 30, -1, -1}`，原有前缀保留 | 容量不足时扩张；缩短长度不缩容量 |
| `a.reserve(5)` | 3 | `{10, 20, 30}`，不创建新元素 | 容量至少为 5；已有容量足够则不变 |
| `reverse(a.begin(), a.end())` | 3 | `{30, 20, 10}`，只改变顺序 | 不变 |

> **替换整份内容用 `assign`；保留前缀、调整长度用 `resize`；只预留空间用 `reserve`；反转顺序用 `reverse`。**

`reverse` 是 `<algorithm>` 中的标准算法，不是 `vector` 成员函数，时间为 $O(n)$；与 `reserve` 是两个不同的名字。详细用法见[反转算法](../algorithms/06-reverse-rotate.md#一reverse)。

#### 为什么重置状态不能只用 `resize`

```cpp
vector<int> color = {0, 1, 0};

color.resize(3, -1); // 长度没有变化，原内容仍为 {0, 1, 0}
color.resize(5, -1); // 只有新增位置为 -1，得到 {0, 1, 0, -1, -1}

color.assign(5, -1); // 所有位置重新设为 -1，得到 5 个 -1
```

距离、颜色或频次表需要**重新确定长度并重置全部状态**时，`assign(n, value)` 更直接。若只想修改现有元素值而保持长度，可使用 `fill(a.begin(), a.end(), value)`。

`assign` 不承诺释放多余容量，也不能据此假定一定重新分配内存；是否保留存储空间与本次内容重置是两回事。

接口参考：[Microsoft Learn · vector::assign](https://learn.microsoft.com/en-us/cpp/standard-library/vector-class#assign)。

## 4. 代表题：[CF 1560A — Dislike of Threes](https://codeforces.com/problemset/problem/1560/A)

**题型：预处理 + 第 k 项查询。** 查询第 k 个既不能被 3 整除、个位又不为 3 的正整数，`k <= 1000`，有多组询问。

### 4.1 建模

所有询问针对同一条固定序列，因此先生成前 1000 个合法整数存进 `vector`，之后每次直接通过 `a[k - 1]` 查询。

```text
枚举候选数 → 排除不合法值 → push_back → 按下标回答
```

### 4.2 独立实现与精简

原实现先手动加入 `1`、`2`，再从 `4` 开始枚举，已经正确 AC：

```cpp
vector<int> a;

void pre() {
    a.push_back(1);
    a.push_back(2);

    for (int i = 4; a.size() < 1000; ++i) {
        if (i % 3 == 0 || i % 10 == 3) continue;
        a.push_back(i);
    }
}

void solve() {
    int k;
    cin >> k;
    cout << a[k - 1] << '\n';
}
```

**唯一值得改的地方：去掉不必要的特殊初始化。** 把 `pre()` 改为：

```cpp
void pre() {
    for (int i = 1; a.size() < 1000; ++i) {
        if (i % 3 == 0 || i % 10 == 3) continue;
        a.push_back(i);
    }
}
```

在 `main()` 中读取询问次数后、进入多组 `solve()` 循环前，**只调用一次 `pre()`**。不必重复展示几乎相同的完整程序。

### 4.3 复杂度与收获

设预处理检查了 M 个候选整数、共有 T 次查询：

- 预处理：O(M)，存储前 1000 个合法数。
- 单次查询：O(1)。
- 总时间：O(M + T)；存储空间：O(1000)。

这次改进不改变复杂度，优化的是**统一判定逻辑，减少特殊分支**。

## 5. 选型触发器

> **动态收集元素 + 尾部添加为主 + 需要随机访问 → 优先考虑 `vector`。**

> **多个询问针对同一个可提前生成的序列 → 一次预处理存入 `vector`，之后通过下标 O(1) 回答。**

若频繁在首端插入、删除，考虑 `deque`；若只关心 FIFO / LIFO 操作，可考虑 `queue` / `stack`。
