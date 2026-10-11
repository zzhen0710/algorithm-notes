# 1.4.10 `unordered_set`：哈希集合

## 1. 核心定位

`unordered_set` 基于哈希表，**元素唯一，但不维护顺序**。适合频繁判断某个值是否出现过，而不需要按大小排序、查询前驱或后继的场景。

```cpp
unordered_set<int> s;

s.insert(5);
s.insert(2);
s.insert(5);      // 重复元素不会再次插入

cout << s.size(); // 2
cout << s.count(2); // 1
```

- `set`：有序、不重复，插入与查找为 `O(log n)`。
- `unordered_set`：无序、不重复，插入与查找平均 `O(1)`，最坏 `O(n)`。
- `multiset`：有序、允许重复。

**关键：哈希集合只负责快速判断是否存在；不要依赖它的遍历顺序。**

## 2. 常用 API

以 `unordered_set<int>` 为例，以下是 **C++11 常用接口签名**；省略少见重载及部分 `const` 版本。

| API 签名 | 作用 / 返回值 | 平均复杂度 | 最坏复杂度 |
|---|---|---|---|
| `pair<iterator, bool> insert(const int& value)` | 插入元素；返回位置与是否成功插入 | `O(1)` | `O(n)` |
| `template<class... Args> pair<iterator, bool> emplace(Args&&... args)` | 用参数构造并尝试插入；返回位置与成功标记 | 均摊 `O(1)` | `O(n)` |
| `iterator find(const int& key)` | 查找元素；不存在返回 `end()` | `O(1)` | `O(n)` |
| `size_type count(const int& key) const` | 返回 `0` 或 `1`，判断是否存在 | `O(1)` | `O(n)` |
| `size_type erase(const int& key)` | 按值删除；返回 `0` 或 `1` | `O(1)` | `O(n)` |
| `iterator erase(const_iterator pos)` | 删除指定元素；返回下一迭代器 | `O(1)` | `O(n)` |
| `size_type size() const` | 当前元素数量 | `O(1)` | `O(1)` |
| `bool empty() const` | 判断是否为空 | `O(1)` | `O(1)` |
| `void clear()` | 删除所有元素 | `O(n)` | `O(n)` |
| `void reserve(size_type count)` | 为预计元素数预留足够的桶空间 | `O(n)` | `O(n²)` |

这里的 `n` 表示当前容器元素数。哈希冲突严重时，查找、插入及删除可能退化为线性时间；`reserve()` 重建桶时也存在最坏退化，**平均 `O(1)` 不等于每次操作都保证 `O(1)`**。

### 2.1 `insert()` 的返回值

```cpp
unordered_set<int> seen;

auto result = seen.insert(7);
cout << result.second;  // 1：首次插入成功

result = seen.insert(7);
cout << result.second;  // 0：已存在，没有再次插入
```

- `result.first`：元素对应的迭代器。
- `result.second`：是否**新插入**元素。

因此，如果操作是“只处理首次出现的元素”，可以直接使用：

```cpp
if (seen.insert(x).second) {
    // x 第一次出现
}
```

这是 C++11 写法，无需 C++17 结构化绑定。

`emplace()` 同样返回插入结果，可利用参数构造元素；重复值仍不会插入：

```cpp
auto result = seen.emplace(8);
if (result.second) {
    // 8 首次出现
}
```

`emplace()` 不保证一定比 `insert()` 快，且元素重复时也可能先构造临时对象。

### 2.2 `find()`、`count()` 与删除

```cpp
unordered_set<int> seen = {2, 5, 8};

if (seen.count(5)) {
    // 5 已存在
}

auto it = seen.find(8);
if (it != seen.end()) {
    seen.erase(it);
}

seen.erase(2);  // 按值删除；不存在时也可调用
```

`unordered_set` 不允许重复，所以按值删除最多只会删掉一个元素；这与 `multiset::erase(key)` 可能删除**全部相等元素**不同。

## 3. 高频陷阱

### 3.1 无序，不代表不能处理保序问题

```cpp
unordered_set<int> seen = {5, 2, 8, 1};

for (int x : seen) {
    cout << x << ' '; // 顺序不保证
}
```

> 如果题目要求保留输入顺序，**应当按输入顺序处理数据，而不是依赖集合遍历顺序**。

### 3.2 不支持有序查询

`unordered_set` 没有 `lower_bound()`、`upper_bound()`，不能直接查询前驱、后继，也没有 `s[i]`。

需要动态维护大小顺序时，应考虑 `set` 或 `multiset`。

### 3.3 迭代器可能失效

插入元素触发 `rehash` 时，已保存的迭代器会失效；显式调用 `reserve()` / `rehash()` 也可能触发重哈希。

`erase(it)` 会使被删元素的迭代器失效。`end()` 不能解引用，也不能传给 `erase()` 当作待删除位置。

## 4. 代表题：[P4305 [JLOI2011] 不重复数字](https://www.luogu.com.cn/problem/P4305)

**题型**：哈希判重 / 保留首次出现顺序 / 多组测试。

给定若干整数，删除重复值，只保留每个数**首次出现的位置与原始顺序**。

例如：

```text
输入序列：5 2 8 2 5 7 8 1
保序去重：5 2 8 7 1
```

### 4.1 核心观察

`unordered_set` 虽然无序，但题目并不要求按集合内部顺序输出。

可以把两项职责分开：

```text
原始输入顺序 → 决定答案顺序
unordered_set → 只判断数字是否出现过
```

**按输入顺序扫描，遇到未出现的数就立即输出并记录；已出现则跳过。**

### 4.2 初始实现

```cpp
void solve() {
    int n;
    cin >> n;

    unordered_set<int> s;

    while (n--) {
        int t;
        cin >> t;

        if (s.count(t)) continue;

        cout << t << ' ';
        s.insert(t);
    }

    cout << '\n';
}
```

这份实现正确：边读边输出，原始顺序天然得到保留；集合只用于判重。每组在 `solve()` 内创建新集合，避免多组测试之间状态污染。

### 4.3 小幅简化：一次插入同时完成判重

初始版本先 `count()` 再 `insert()`，首次出现的元素需要进行两次哈希查找。

只修改循环主体即可：

```cpp
while (n--) {
    int t;
    cin >> t;

    // insert().second 为 true 表示此前未出现
    if (s.insert(t).second) {
        cout << t << ' ';
    }
}
```

两种写法渐近复杂度相同；简化版减少重复查询，并直接利用 `insert()` 的返回值表达“首次出现”。

**复杂度**：平均时间 `O(n)`，哈希严重退化时最坏 `O(n²)`；空间 `O(n)`。

## 5. 选型触发器

> **只需要高效判断某个键是否已出现，不关心集合内元素的排序 → `unordered_set`。**

**如果还要求“保留首次出现顺序”，不必因此放弃哈希：**

> **由输入遍历负责顺序，由哈希集合负责存在性判断。**

与 P1540 的 `queue + bool[]` 类似，这是让不同处理环节各司其职，而不是强迫单个容器承担全部需求。
