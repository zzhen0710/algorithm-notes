# 1.4.9 `multiset`：有序可重复集合

## 1. 核心定位

`multiset` 默认按升序维护元素，**允许重复值**，通常由平衡搜索树实现。

```cpp
multiset<int> ms = {5, 2, 5, 8};
// 遍历顺序：2 5 5 8

cout << *ms.begin();   // 2，最小值
cout << *ms.rbegin();  // 8，最大值
```

**适用场景**：动态插入、删除**某个值的一次出现**，并随时查询最值、前驱、后继；数据允许重复且必须保留重复次数。

- `set`：有序，**不重复**。
- `multiset`：有序，**可重复**。
- `priority_queue`：可重复，但只能方便地访问、删除堆顶；不支持任意指定值的删除。
- `unordered_multiset`：允许重复，但不维护大小顺序（后续需要时再学）。

## 2. 常用 API

以下以 `multiset<int>` 为例，列出 **C++11 常用接口签名**，省略部分重载及 `const` 版本。`iterator`、`const_iterator`、`reverse_iterator`、`size_type` 均为容器定义的类型。

| API 签名 | 作用 / 返回值 | 时间复杂度 |
|---|---|---|
| `iterator insert(const int& value)` | 插入一个元素，返回新元素位置；**不会去重** | \(O(\log n)\) |
| `template<class... Args> iterator emplace(Args&&... args)` | 用参数构造并插入一个元素，返回新元素位置；**允许重复** | \(O(\log n)\) |
| `iterator find(const int& key)` | 查找某个等于 `key` 的元素；不存在返回 `end()` | \(O(\log n)\) |
| `size_type count(const int& key) const` | 返回等于 `key` 的元素个数 | \(O(\log n+k)\) |
| `size_type erase(const int& key)` | **删除所有**等于 `key` 的元素，返回删除数量 | \(O(\log n+k)\) |
| `iterator erase(const_iterator pos)` | **只删除**迭代器指向的一个元素，返回下一位置 | 均摊 \(O(1)\) |
| `iterator lower_bound(const int& key)` | 第一个 `>= key` 的元素位置 | \(O(\log n)\) |
| `iterator upper_bound(const int& key)` | 第一个 `> key` 的元素位置 | \(O(\log n)\) |
| `pair<iterator, iterator> equal_range(const int& key)` | 返回所有等值元素构成的左闭右开区间 | \(O(\log n)\) |
| `iterator begin()` / `iterator end()` | 最小元素位置 / 尾后位置 | \(O(1)\) |
| `reverse_iterator rbegin()` | 最大元素位置 | \(O(1)\) |
| `size_type size() const` / `bool empty() const` | 元素数量（含重复）/ 是否为空 | \(O(1)\) |
| `void clear()` | 删除所有元素 | \(O(n)\) |

其中 \(n\) 为当前元素数量，\(k\) 为等于 `key` 的元素数量。表中复杂度默认整数比较为 \(O(1)\)。

`emplace()` 与 `insert()` 都会保留重复元素，但 `multiset::emplace()` 返回的是 **`iterator`**，不是 `pair<iterator, bool>`：

```cpp
multiset<int> ms;
auto it = ms.emplace(5);  // 插入一个 5，it 指向新元素
ms.emplace(5);             // 再插入一个 5；重复值不会被拒绝
```

### 2.1 删除一个值，还是删除全部相同值？

```cpp
multiset<int> ms = {2, 5, 5, 5, 8};

ms.erase(5);  // 按值删除：三个 5 全部删除，剩下 {2, 8}
```

如果只允许删除**一个** `5`：

```cpp
multiset<int> ms = {2, 5, 5, 5, 8};

auto it = ms.find(5);
if (it != ms.end()) {
    ms.erase(it);  // 只删除一个 5，剩下 {2, 5, 5, 8}
}
```

> **`erase(key)` 删所有相等元素；`erase(iterator)` 只删一个。先检查 `find()` 结果，不要对 `end()` 调用 `erase()`。**

### 2.2 等值区间、上下界与距离

```cpp
multiset<int> ms = {1, 3, 3, 3, 7};

auto l = ms.lower_bound(3);  // 第一个 3
auto r = ms.upper_bound(3);  // 7：第一个 > 3 的元素

// [l, r) 恰好包含全部三个 3
cout << ms.count(3);         // 3
```

也可以使用：

```cpp
auto range = ms.equal_range(3);
// range.first == lower_bound(3)
// range.second == upper_bound(3)
```

注意：`multiset` 的迭代器不是随机访问迭代器。

```cpp
distance(l, r);  // O(k)：需要逐个前进，不是 O(1)
```

`std::lower_bound(ms.begin(), ms.end(), x)` 的迭代器移动可能达到 \(O(n)\)；要找上下界，使用成员函数 `ms.lower_bound(x)`，保证 \(O(\log n)\)。

### 2.3 迭代器与边界

- `begin()` / `rbegin()` 取最值之前，必须保证集合非空。
- `end()` 不能解引用；`begin()` 不能继续 `--`。
- `insert()` 不会使其他元素的迭代器失效；`erase(it)` 只使被删元素的迭代器失效。
- 与 `set` 一样，不支持 `ms[i]`，也不能直接修改元素使其改变排序位置。

---

## 3. 代表题：[CF 1029C — Maximal Intersection](https://codeforces.com/problemset/problem/1029/C)

**题型**：区间交集 / 动态维护极值 / 枚举删除一个对象 / 离线预处理优化。

**题意**：给定 \(n\) 个区间，**恰好删除一个区间**，最大化其余区间的公共交集长度。允许端点重复，区间也可能退化为单点。

### 3.1 首先找出决定答案的状态

任意一组区间的交集，只取决于：

\[
L=\max_i l_i,\qquad R=\min_i r_i
\]

其长度为：

\[
\max(0,R-L)
\]

因此，无须维护或检查所有可能的交集内部位置，只需要维护**最大左端点**与**最小右端点**。

> 这里说的是数值大小，与“离 0 远近”无关；负坐标也适用。

### 3.2 方案一：双 `multiset` 动态维护

用两个多重集合分别维护全部左端点与右端点：

```cpp
vector<int> vl(n), vr(n);
multiset<int> ml, mr;

for (int i = 0; i < n; ++i) {
    cin >> vl[i] >> vr[i];
    ml.insert(vl[i]);
    mr.insert(vr[i]);
}
```

对每个候选删除区间：

```cpp
// Temporarily remove one interval; duplicate endpoints must be preserved.
ml.erase(ml.find(vl[i]));
mr.erase(mr.find(vr[i]));

// The intersection is determined by max(left) and min(right).
int L = *ml.rbegin();   // O(1)
int R = *mr.begin();    // O(1)
ans = max(ans, R - L);

// Restore the interval before examining the next candidate.
ml.insert(vl[i]);
mr.insert(vr[i]);
```

**完整过程**：

```text
确定交集只依赖两个极值
→ 用双 multiset 分别维护左右端点
→ 枚举待删除的区间
→ 临时删除其两个端点（各删一个）
→ O(1) 查询剩余交集边界
→ 更新答案并恢复现场
```

`n >= 2`，所以删除一个区间后两个 `multiset` 仍非空，可以安全读取 `begin()` / `rbegin()`。

#### 两处小幅修正

**① `ans` 直接初始化为 0。** 初始实现先计算了未删除任何区间的交集：

```cpp
int l = *ml.rbegin();
int r = *mr.begin();
int ans = max(0, r - l);
```

这并非错误，但题目只关心**删除恰好一个区间之后**的答案；且长度的合法下界是 0，因此直接：

```cpp
int ans = 0;
```

循环内 `ans = max(ans, R - L)` 就已同时保证结果非负。这里的 0 是**求最大值的合法初始下界**。

**② 注释里应把复杂度归因于正确的操作。**

```cpp
// Insight: Intersection = [max(left endpoints), min(right endpoints)].
// Approach: Remove one interval, query both extrema, then restore it.
// Erase by iterator to preserve duplicate endpoints.
// Complexity: find/insert O(log n), extrema O(1), total O(n log n).
```

**总复杂度**：初始化 \(O(n\log n)\)，每轮查找、删除、插回 \(O(\log n)\)，共 \(n\) 轮，所以时间 \(O(n\log n)\)，额外空间 \(O(n)\)。

### 3.3 进一步优化：静态数据不必动态删除

真正的题目结构是：**所有区间预先给出，只枚举“删掉哪个区间”，并不永久修改集合。**

删除第 \(i\) 个区间，剩余部分可以拆成：

```text
[1 ... i-1]   删除 i   [i+1 ... n]
```

若提前保存每个前缀与后缀的：

- 最大左端点；
- 最小右端点；

那么删除 \(i\) 后的两个边界可在 \(O(1)\) 内求出：

\[
L_i=\max(preL[i-1],sufL[i+1])
\]
\[
R_i=\min(preR[i-1],sufR[i+1])
\]

#### O(n) 实现：前后缀极值

```cpp
void solve() {
    int n;
    cin >> n;

    // 使用 1-based 下标，额外预留 0 和 n+1 作为空区间边界
    vector<int> l(n + 2), r(n + 2);
    for (int i = 1; i <= n; ++i)
        cin >> l[i] >> r[i];

    // preL[i] / preR[i]：区间 [1, i] 的最大左端点 / 最小右端点
    // sufL[i] / sufR[i]：区间 [i, n] 的最大左端点 / 最小右端点
    // 空区间初始化为单位元：max 用 INT_MIN，min 用 INT_MAX
    vector<int> preL(n + 2, INT_MIN), preR(n + 2, INT_MAX);
    vector<int> sufL(n + 2, INT_MIN), sufR(n + 2, INT_MAX);

    // 从左到右计算前缀极值
    for (int i = 1; i <= n; ++i) {
        preL[i] = max(preL[i - 1], l[i]);
        preR[i] = min(preR[i - 1], r[i]);
    }

    // 从右到左计算后缀极值
    for (int i = n; i >= 1; --i) {
        sufL[i] = max(sufL[i + 1], l[i]);
        sufR[i] = min(sufR[i + 1], r[i]);
    }

    int ans = 0;  // 交集长度不可能为负
    for (int i = 1; i <= n; ++i) {
        // 删除第 i 个区间：只保留 [1, i-1] 和 [i+1, n]
        int L = max(preL[i - 1], sufL[i + 1]);  // 剩余最大左端点
        int R = min(preR[i - 1], sufR[i + 1]);  // 剩余最小右端点

        // 当 R < L 时无交集；ans 初值为 0，直接取 max 即可
        ans = max(ans, R - L);
    }

    cout << ans << '\n';
}
```

前后缀数组的 `INT_MIN` / `INT_MAX` 是**求最大值 / 最小值的空区间单位元**，使删除首尾区间时不必单独写边界分支。

这版时间 \(O(n)\)，空间 \(O(n)\)；相较双 `multiset`，优化来源于**利用离线已知的完整数据，预处理排除某一项后的结果**，而不是更快的 STL 接口。

### 3.4 两种解法应该如何选？

| 方案 | 时间 | 空间 | 适用场景 |
|---|---|---|---|
| 双 `multiset` | \(O(n\log n)\) | \(O(n)\) | 集合真正动态变化，需要插入、删除并维护极值 |
| 前后缀极值 | \(O(n)\) | \(O(n)\) | 全部数据已知，只需枚举排除一个位置 |

> **先提取决定答案的极值，再判断是否真需要动态数据结构；若所有数据已知，尝试用前后缀预处理替代动态修改。**

---

## 4. 高频错误速查

| 错误 | 原因 | 正确做法 |
|---|---|---|
| `ms.erase(x)` 只想删一个 `x` | 实际会删掉全部相等值 | `auto it=ms.find(x); if(it!=ms.end()) ms.erase(it);` |
| 把 `multiset` 当成支持下标的数组 | 树的迭代器不支持随机访问 | 用 `begin()`、`rbegin()`、`lower_bound()` |
| 空集合上解引用 `begin()` / `rbegin()` | 没有任何元素 | 先判断 `empty()` |
| 认为 `distance(l,r)` 在树上是 \(O(1)\) | 双向迭代器需要逐步移动 | 复杂度为 \(O(k)\) |
| 在 `multiset` 上调用算法版 `lower_bound` | 迭代器移动可能为线性 | 使用 `ms.lower_bound(x)` |
| 把取极值误写成 \(O(\log n)\) | 混淆了查找与端点迭代器 | `*ms.begin()` / `*ms.rbegin()` 为 \(O(1)\) |

## 5. 选型触发器

```text
有重复值
+
集合持续动态变化
+
需要按值删一个 / 查最值 / 查询前驱后继
→ multiset
```

若只关心反复弹出堆顶，优先考虑 `priority_queue`；若全部数据已知、只枚举删除一个元素，检查是否能用**前后缀 / 极值备选信息**优化。

**一句话总结：`multiset` 解决“动态、有序、可重复”；CF 1029C 则进一步说明，找到答案的关键状态之后，还应思考能否用静态预处理消除动态维护。**
