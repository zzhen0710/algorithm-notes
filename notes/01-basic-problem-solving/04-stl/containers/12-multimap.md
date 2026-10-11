# 1.4.12 `multimap`：有序可重复键值映射

## 1. 核心定位

`multimap<Key, Value>` 是**按 key 有序、允许多个相同 key** 的关联容器，通常由平衡搜索树实现。

```cpp
multimap<string, int> scores;

scores.emplace("Alice", 90);
scores.emplace("Bob", 85);
scores.emplace("Alice", 95);  // 相同 key 可以对应多个 value

cout << scores.count("Alice");  // 2
```

- `map`：每个 key 最多对应一个 value。
- `multimap`：同一个 key 可以对应多个 value，元素仍按 key 有序。
- `unordered_map`：key 唯一，不维护 key 的大小顺序，增删查平均 `O(1)`、最坏 `O(n)`。

**注意：`multimap` 没有 `operator[]`，也没有 `at()`。** 对一个可能对应多个 value 的 key，无法通过下标确定要访问哪一个值。

## 2. 常用 API

以 `multimap<string, int>` 为例，列出 **C++11 常用接口签名**，省略不常用重载及部分 `const` 版本。`value_type` 为 `pair<const string, int>`；`iterator`、`const_iterator`、`size_type` 为容器成员类型。

| API 签名 | 作用 / 返回值 | 时间复杂度 |
|---|---|---|
| `iterator insert(const value_type& value)` | 插入键值对，返回新元素位置；**不拒绝重复 key** | `O(log n)` |
| `template<class... Args> iterator emplace(Args&&... args)` | 用参数构造并插入键值对，返回新元素位置 | `O(log n)` |
| `iterator find(const string& key)` | 返回某个匹配 key 的元素位置；不存在返回 `end()` | `O(log n)` |
| `size_type count(const string& key) const` | 返回该 key 对应的键值对数量 | `O(log n + k)` |
| `pair<iterator, iterator> equal_range(const string& key)` | 返回所有匹配 key 元素的左闭右开区间 | `O(log n)` |
| `iterator lower_bound(const string& key)` | 返回首个 key 不小于目标值的位置 | `O(log n)` |
| `iterator upper_bound(const string& key)` | 返回首个 key 大于目标值的位置 | `O(log n)` |
| `size_type erase(const string& key)` | 删除**所有**匹配 key 的元素，返回删除个数 | `O(log n + k)` |
| `iterator erase(const_iterator pos)` | 只删除指定的一项，返回下一元素位置 | 均摊 `O(1)` |
| `iterator begin()` / `iterator end()` | 起始位置 / 尾后位置 | `O(1)` |
| `size_type size() const` / `bool empty() const` | 元素总数（含重复 key）/ 是否为空 | `O(1)` |
| `void clear()` | 清空所有元素 | `O(n)` |

其中 `n` 为当前元素总数，`k` 为与指定 key 相等的元素数；复杂度默认比较单个 key 的成本为 `O(1)`，若 key 为字符串，还需计入字符串比较成本。

### 2.1 `equal_range()`：一次取得同一 key 的全部值

```cpp
multimap<string, int> scores;
scores.emplace("Alice", 90);
scores.emplace("Bob", 85);
scores.emplace("Alice", 95);

auto range = scores.equal_range("Alice");
for (auto it = range.first; it != range.second; ++it) {
    cout << it->first << ' ' << it->second << '\n';
}
// Alice 90
// Alice 95
```

相等的 key 在有序容器中连续排列，因此可用 `[range.first, range.second)` 遍历全部匹配项。查询边界为 `O(log n)`，遍历 `k` 项另外需要 `O(k)`。从 C++11 起，同一 key 的等价元素保持相对插入顺序。

### 2.2 按 key 删除，与按迭代器删除

```cpp
multimap<string, int> scores;
scores.emplace("Alice", 90);
scores.emplace("Alice", 95);

// 只删除其中一条记录
// 必须确认迭代器不是 end()
auto it = scores.find("Alice");
if (it != scores.end()) scores.erase(it);

// 删除剩下所有 key 为 Alice 的记录
scores.erase("Alice");
```

与 `multiset` 相同，**`erase(key)` 删除所有匹配项，`erase(iterator)` 只删除一项**。相反，`insert()` 和 `emplace()` 始终允许相同 key，故返回 `iterator`，而非 `pair<iterator, bool>`。

## 3. 选型与典型用途

`multimap` 常用于**一个 key 对应多条记录，同时希望所有记录按 key 排序、且能对单条记录单独插入或删除**的场景。

例如，同一时间戳下可能产生多条事件；或按学生姓名排列多门课程记录。前面“一个姓名对应多项成绩”的示例就体现了其用法。

但在竞赛中，若主要需求是“按 key 直接访问一整组值”，很多时候更适合：

```cpp
map<string, vector<int>> scores;
scores["Alice"].push_back(90);
scores["Alice"].push_back(95);

for (int value : scores["Alice"]) {
    cout << value << ' ';
}
```

这两种表示并不完全等价：`map<Key, vector<Value>>` 方便整组访问，`multimap` 则让每一对 key/value 都作为单独的有序树节点存在。

`multimap` 没有必须专门使用它才能解决的高频入门题，因此本节只保留接口与选型，不强行安排独立代表题。

> **需要按 key 排序，并允许一个 key 对应多条独立记录 → 考虑 `multimap`；若经常按 key 获取整组值 → 也考虑 `map<Key, vector<Value>>`。**
