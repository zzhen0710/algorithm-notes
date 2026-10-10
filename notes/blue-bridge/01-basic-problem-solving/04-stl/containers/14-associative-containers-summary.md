# 1.4.14 关联容器总览：`set` / `map` 八种类型

## 1. 不死记名称：用三个维度定位容器

C++11 的八种关联容器，区别主要来自三个问题：

1. **集合还是映射？** 集合只保存元素（key）；映射保存 `key → value`。
2. **有序还是哈希？** 有序容器按 key 排列、支持上下界；哈希容器不保证遍历顺序，但增删查通常更快。
3. **key 是否允许重复？** 不带 `multi` 的容器 key 唯一；带 `multi` 的容器允许同一个 key 出现多次。

| 数据性质 | Key 唯一 | Key 可重复 |
|---|---|---|
| **有序集合** | `set<T>` | `multiset<T>` |
| **哈希集合** | `unordered_set<T>` | `unordered_multiset<T>` |
| **有序映射** | `map<K, V>` | `multimap<K, V>` |
| **哈希映射** | `unordered_map<K, V>` | `unordered_multimap<K, V>` |

- **有序**指按 key 的比较规则排序，默认升序，通常以平衡搜索树实现。
- **哈希**指通过哈希表组织数据；**不能依赖遍历次序**。
- 映射的 key 是否唯一，与 **value 是否重复无关**：不同 key 可以对应相同 value。
- `multi` 允许的是重复**元素或 key**，不是同一个 key 对应的 value 被自动装进某个容器。

## 2. 选型速查与复杂度

`n` 为容器中的元素总数；`k` 为某个 key 的匹配元素数。下表不计字符串比较、哈希计算等 key 自身的开销。

| 容器 | 插入 / 查找单项 | 特别擅长的操作 | 竞赛中常见的替代写法 |
|---|---|---|---|
| `set` | `O(log n)` | 动态去重、前驱/后继、上下界 | — |
| `multiset` | `O(log n)` | 保留重复值、动态查询最值、只删一个重复项 | — |
| `unordered_set` | 平均 `O(1)`，最坏 `O(n)` | 快速判重、记录是否出现 | 范围小且连续时可用 `bool[]` |
| `unordered_multiset` | 平均 `O(1)`，最坏 `O(n)` | 无序保存重复项、按迭代器删一项 | `unordered_map<T, int>` 计数 |
| `map` | `O(log n)` | 有序的 key → value 映射、上下界 | — |
| `multimap` | `O(log n)` | 同一 key 的多条独立记录，按 key 有序 | `map<K, vector<V>>` |
| `unordered_map` | 平均 `O(1)`，最坏 `O(n)` | 频率统计、维护 key 对应状态 | 范围小且连续时可用数组 |
| `unordered_multimap` | 平均 `O(1)`，最坏 `O(n)` | 无序的一对多独立记录 | `unordered_map<K, vector<V>>` |

哈希插入的平均 `O(1)` 应理解为通常的**均摊复杂度**；哈希冲突严重时，单次插入、查找和按 key 删除都可能退化到 `O(n)`。

对于 `multi` 容器，还要区分**只找一个匹配项**和**处理全部匹配项**：

| 操作（匹配数为 `k`） | 有序 `multi` | 哈希 `unordered_multi` |
|---|---|---|
| `find(key)`：找一个 | `O(log n)` | 平均 `O(1)`；最坏 `O(n)` |
| `count(key)`：数出全部 | `O(log n + k)` | 平均 `O(1 + k)`；最坏 `O(n)` |
| `equal_range(key)`：取得区间 | `O(log n)` | 平均 `O(1 + k)`；最坏 `O(n)` |
| `erase(key)`：删除全部匹配项 | `O(log n + k)` | 平均 `O(1 + k)`；最坏 `O(n)` |

`equal_range()` 只返回边界；若还要遍历区间内 `k` 项，需再计入 `O(k)`。上表对哈希 `multi` 的平均复杂度采用包含全部匹配元素的保守计法。

## 3. 最容易混淆的 API 行为

### 3.1 `insert()` / `emplace()`：返回类型由“是否唯一”决定

以 `T=int` 或 `K=string, V=int` 为例，省略其他重载。`value_type`：集合是 `T`，映射是 `pair<const K, V>`。

| 容器类型 | `insert(const value_type& value)` | `emplace(Args&&... args)` | 遇到相同 key |
|---|---|---|---|
| 四种**唯一键**容器：`set`、`unordered_set`、`map`、`unordered_map` | `pair<iterator, bool>` | `pair<iterator, bool>` | 不插入、不覆盖 |
| 四种 **`multi`** 容器 | `iterator` | `iterator` | 再插入一条记录 |

`emplace()` 是 `template<class... Args>` 成员函数，可直接用构造参数尝试插入，但不保证一定更快；重复 key 时也可能产生临时构造成本。

```cpp
set<int> a;
auto inserted = a.emplace(5);
cout << inserted.second;   // 1：本次成功插入新 key

multiset<int> b;
auto it = b.emplace(5); // 返回 iterator
b.emplace(5);             // 第二个 5 依然插入
```

### 3.2 `erase(key)`：唯一键删至多一个，`multi` 删除全部

所有八种容器都支持：

- `size_type erase(const key_type& key)`：返回**实际删除数量**。唯一键容器结果为 0 或 1；`multi` 可能大于 1。
- `iterator erase(const_iterator pos)`：只删除**迭代器所指的一项**，返回下一位置；传入前应检查 `pos != end()`。

```cpp
unordered_multiset<int> bag = {5, 5, 5, 8};

auto it = bag.find(5);
if (it != bag.end()) bag.erase(it); // 只删一个 5

bag.erase(5);  // 剩余的 5 全部删除
```

### 3.3 `operator[]`：只有 `map` / `unordered_map` 支持

| 容器 | `operator[]` / `at()` | 原因 |
|---|---|---|
| `map`、`unordered_map` | 两者都有 | 每个 key 唯一，可以定位一个 value |
| `multimap`、`unordered_multimap` | 都没有 | 同一 key 可对应多条记录，访问哪一条不明确 |
| 四种集合 | 都没有 | 集合不存储独立的 mapped value，也不支持按下标访问 |

```cpp
unordered_map<string, int> cnt;
cout << cnt["cat"]; // 输出 0，同时新插入了 {"cat", 0}
++cnt["cat"];       // 频率统计适用

// 只查不插入：
auto it2 = cnt.find("dog");
if (it2 != cnt.end()) cout << it2->second;
```

`at(key)` 不会自动插入，key 不存在会抛异常；如果仅判断存在，优先 `count()` / `find()`。

### 3.4 顺序与区间：`lower_bound` 不等于 `equal_range`

- 只有**有序四种**支持成员函数 `lower_bound(key)`、`upper_bound(key)`，分别寻找首个 `>= key` 与首个 `> key` 的元素（默认升序）。
- **八种容器都支持 `equal_range(key)`**，返回 `pair<iterator, iterator>`（只读容器对应 `const_iterator`），表示所有等价 key 的左闭右开区间。
- 哈希容器的 `equal_range()` 不是大小范围查询；它只取得哈希表中**相同 key 的记录区间**。
- 有序容器可通过 `begin()` / `rbegin()` 得到最小 / 最大 key（需非空）；哈希容器的首尾迭代器不代表最值。

```cpp
unordered_multimap<string, int> scores;
scores.emplace("Alice", 90);
scores.emplace("Alice", 95);
scores.emplace("Bob", 88);

auto range = scores.equal_range("Alice");
for (auto it3 = range.first; it3 != range.second; ++it3) {
    cout << it3->second << ' '; // 90 和 95，不能依赖整体遍历顺序
}
```

此外，哈希容器可用 `reserve()` / `rehash()` 管理桶；插入时若触发 `rehash`，旧迭代器可能失效。有序树容器插入通常不会使已有元素的迭代器失效。

## 4. 两个冷门容器到底什么时候用？

### `unordered_multiset<T>`

**有重复值，但完全不需要大小顺序**。如果每次插入都需要成为一条独立记录，或需要保存某一项的迭代器并单独删除，可考虑它。

若需求本质是“每个数字出现几次”，通常用 `unordered_map<T, int>` 计数更直观；但计数映射与多重集合并非完全等价，特别是元素逐项迭代、定位单条记录时。

### `unordered_multimap<K, V>`

**一个 key 对应多条独立记录，但不需要按 key 排序**。借助 `equal_range()` 遍历该 key 的所有记录。

若需求本质是“直接获取这个 key 对应的一整组值”，`unordered_map<K, vector<V>>` 往往更方便：

```cpp
unordered_map<string, vector<int>> group;
group["Alice"].push_back(90);
group["Alice"].push_back(95);

for (int score : group["Alice"]) {
    cout << score << ' ';
}
```

这两种结构不完全等价：前者把每条键值对当作独立元素，后者把整组 `vector` 绑定到一个 key。

## 5. 比记名称更实用的选型顺序

```text
只需判断存在？
  ├─ 不要求大小顺序 → unordered_set
  └─ 需要排序 / 前驱后继 → set

需要保存 key → value？
  ├─ key 唯一
  │   ├─ 无须按 key 排序 → unordered_map
  │   └─ 需要按 key 排序 → map
  └─ 一个 key 对应多个 value
      ├─ 经常整组读取 → map<K, vector<V>> / unordered_map<K, vector<V>>
      └─ 每条记录需独立存在 → multimap / unordered_multimap

只保存值，但允许重复且需要动态维护最值或有序查询？
  └─ multiset

只保存值、允许重复且不要求排序？
  └─ 先考虑 unordered_map<T, int> 计数；需要逐条保存时再考虑 unordered_multiset
```

**核心原则：先明确需要什么操作，再选容器；不是所有带 `multi` 的场景都必须使用 `multi` 容器。**

## 6. 与单容器笔记的关系

这份笔记负责**横向比较、快速选型和查易错 API**，不重复展开代表题。典型题及完整代码仍保留在对应的单容器笔记中：

- [`08-set.md`](./08-set.md)：P5250 木材仓库；前驱、后继与最近值。
- [`09-multiset.md`](./09-multiset.md)：CF 1029C；重复端点、删除一个、前后缀优化。
- [`10-unordered-set.md`](./10-unordered-set.md)：P4305；哈希判重与输入保序。
- [`11-map.md`](./11-map.md)：P5266；键值映射的增删改查。
- [`12-multimap.md`](./12-multimap.md)：重复 key、`equal_range()` 与一对多存储。
- [`13-unordered-map.md`](./13-unordered-map.md)：CF 4C；记录最小充分状态，避免重复试探。

> **一句话总结：集合 / 映射决定“存什么”，有序 / 哈希决定“怎么查”，唯一 / `multi` 决定“重复 key 怎么处理”。**
