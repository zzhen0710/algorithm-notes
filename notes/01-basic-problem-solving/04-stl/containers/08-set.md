# 1.4.8 `set`：有序且不重复的集合

## 1. 定位与选型

`set` 默认按升序保存**不重复**的元素，通常由平衡搜索树实现。它不仅适合动态判重，也适合寻找前驱、后继和最接近目标的值。

```cpp
set<int> s = {5, 2, 8, 2};  // 2 5 8：自动有序、去重

cout << *s.begin();          // 2，最小值
cout << *s.rbegin();         // 8，最大值
```

- **选 `set`**：动态插入、按值删除、判重，并且需要**有序性**。
- **选 `unordered_set`**：主要做判重或存在性查询，不需要有序性。
- **选 `multiset`**：需要保存重复元素，并维护有序性。

## 2. 常用 API

以下以 `set<int>` 为例。表中的 `iterator`、`const_iterator`、`reverse_iterator`、`size_type` 均指该容器定义的类型；列出的是 **C++11 常用接口签名**，省略其他重载和 `const` 版本。

| API 签名 | 作用 / 返回值 | 复杂度 |
|---|---|---|
| `pair<iterator, bool> insert(const int& value)` | 返回元素位置和是否**新插入** | \(O(\log n)\) |
| `template<class... Args> pair<iterator, bool> emplace(Args&&... args)` | 用参数构造并尝试插入；返回位置和成功标记 | \(O(\log n)\) |
| `iterator find(const int& key)` | 找到对应元素；不存在返回 `end()` | \(O(\log n)\) |
| `size_type count(const int& key) const` | 存在返回 1，否则返回 0 | \(O(\log n)\) |
| `size_type erase(const int& key)` | 按值删除；返回 0 或 1 | \(O(\log n)\) |
| `iterator erase(const_iterator pos)` | 删除迭代器指向的元素，返回其后继 | 均摊 \(O(1)\) |
| `iterator lower_bound(const int& key)` | 第一个大于等于 `key` 的位置 | \(O(\log n)\) |
| `iterator upper_bound(const int& key)` | 第一个严格大于 `key` 的位置 | \(O(\log n)\) |
| `iterator begin()` / `iterator end()` | 最小元素位置 / 尾后位置 | \(O(1)\) |
| `reverse_iterator rbegin()` | 逆序起点，即最大元素的位置 | \(O(1)\) |
| `size_type size() const` / `bool empty() const` | 元素数量 / 判空 | \(O(1)\) |
| `void clear()` | 清空集合 | \(O(n)\) |

### 插入返回值：不用先 `find()` 再 `insert()`

```cpp
set<int> s;

auto result = s.insert(5);
cout << result.second;  // 1：插入成功

auto again = s.insert(5);
cout << again.second;   // 0：元素已存在
```

`result.first` 是对应元素的迭代器；`result.second` 表示是否插入了**新元素**。使用 `auto result` 即可，不依赖 C++17 结构化绑定。

`emplace()` 可用构造参数直接尝试插入，返回值与 `insert()` 相同：

```cpp
auto result = s.emplace(5);  // result.second 表示是否插入成功
```

对于 `int` 等简单类型，两者通常没有明显性能差别；`emplace()` 也不保证在重复元素时完全避免构造开销。

## 3. 前驱、后继与上下界

```cpp
set<int> s = {2, 5, 8, 12};

auto l = s.lower_bound(6);  // 指向 8：第一个 >= 6
auto r = s.upper_bound(8);  // 指向 12：第一个 > 8
```

寻找**严格小于 `x` 的最大元素**（前驱）：

```cpp
auto it = s.lower_bound(x);

if (it != s.begin()) {
    --it;
    cout << *it;
}
```

- `lower_bound(x)` 指向 `begin()`：不存在严格小于 `x` 的元素。
- `lower_bound(x)` 返回 `end()`：集合中所有元素都小于 `x`；若非空，可以先 `--it` 再解引用。
- `lower_bound(x)` 位于中间：它与前一个迭代器分别是大于等于 `x` 的候选和严格小于 `x` 的候选。

> **在有序集合中找最接近 `x` 的数：定位 `lower_bound(x)`，然后比较其前驱与当前元素。**

## 4. 高频陷阱

### 4.1 迭代器边界

```cpp
auto it = s.lower_bound(x);

if (it != s.end()) cout << *it;  // end() 不能解引用
if (it != s.begin()) --it;       // begin() 不能再向前移动
```

`begin()` / `rbegin()` 取最值之前，也必须保证容器非空。

### 4.2 成员函数与通用算法不要混淆

```cpp
auto a = s.lower_bound(x);                      // O(log n)，推荐
auto b = lower_bound(s.begin(), s.end(), x);   // 迭代器移动可达 O(n)
```

通用算法 `std::lower_bound` 在这里的**比较次数**是 \(O(\log n)\)，但 `set` 迭代器不是随机访问迭代器，迭代器推进总次数可能是 \(O(n)\)。

### 4.3 `set` 不支持下标，也不能直接修改元素

```cpp
// s[0];          // 错：set 不支持随机下标访问
// *s.begin()=7;  // 错：不能原地修改集合元素的键值
```

如果要改变某个值，应删除旧值，再插入新值，以保持树的有序结构。

### 4.4 插入与删除的迭代器失效

- `insert()` 不会使已有元素的迭代器失效。
- `erase()` 只使**被删除元素**的迭代器失效；其他元素的迭代器仍有效。
- `set::erase(value)` 在 `set` 中最多删除一个值；但在 `multiset` 中可能删除**所有相等元素**，不可混用。

---

## 5. 代表题：[P5250 〖深基17.例5〗木材仓库](https://www.luogu.com.cn/problem/P5250)

**题型：动态有序集合 / 前驱与后继 / 最近值查询。**

入库时不允许重复长度；取出时选择距离要求长度 `x` 最近的木材，距离相同则选择**较短**的一根，取出后从仓库删除。

### 原始思路（已独立 AC）

```text
查找完全相等的长度
→ 不存在时，用 lower_bound 找第一个 >= x 的候选
→ 同时考虑它的前驱
→ 比较绝对距离（等距优先较短）
→ 删除取出的元素
```

原解中这段边界处理是正确的：

```cpp
auto r = s.lower_bound(len);
auto l = r;
if (r != s.begin()) --l;

if (r == s.end() || r == s.begin()) {
    res = *l;  // 已在前面排除空集合
} else {
    res = (len - *l > *r - len) ? *r : *l;
}
```

对应三种情况：

| `lower_bound(len)` | 可选候选 |
|---|---|
| `begin()` | 只有当前后继 |
| `end()` | 只有前一个元素 |
| 中间位置 | 比较当前元素与其前驱；等距取前驱 |

### 简化：省略额外 `find()`，直接选中最终迭代器

`lower_bound(len)` 如果恰好指向 `len`，其距离就是 0，因此**不必提前单独查找完全相等的值**。

以下是推荐的 `solve()`，保持 C++11 兼容：

```cpp
void solve() {
    int n;
    cin >> n;
    set<int> s;

    while (n--) {
        int op, len;
        cin >> op >> len;

        if (op == 1) {
            auto result = s.insert(len);
            if (!result.second)
                cout << "Already Exist\n";
            continue;
        }

        if (s.empty()) {
            cout << "Empty\n";
            continue;
        }

        auto it = s.lower_bound(len);

        if (it == s.end()) {
            --it;  // 所有木材都比 len 小，取最大值
        } else if (it != s.begin()) {
            auto left = it;
            --left;

            // 距离相等时优先选择更短的木材
            if (len - *left <= *it - len)
                it = left;
        }

        cout << *it << '\n';
        s.erase(it);
    }
}
```

相较原解，优化点在于：

- `insert()` 返回值直接判断是否重复，省去入库前额外 `find()`。
- `lower_bound()` 已自然覆盖“恰好相等”情况，无须取出前再 `find()`。
- 维护**最终选中的迭代器**，可直接 `erase(it)`，不需要按值重新查找。

**时间复杂度：** 共 \(n\) 次操作，每次至多 \(O(\log n)\)，总计 \(O(n\log n)\)；空间 \(O(n)\)。

---

## 6. 选型触发器

> **集合需要动态变化，而且不仅要判断“有没有”，还需要“按大小找邻居” → 想到 `set`。**

尤其是：

```text
动态去重 + 自动排序
动态查询前驱 / 后继
寻找与目标最接近的值
```

若不关心顺序，考虑 `unordered_set`；若重复值必须保留，改用 `multiset`。
