# 1.4.11 `map`：有序键值映射

## 1. 核心定位

`map<Key, Value>` 维护 **key 唯一、按 key 有序**的键值对，通常基于平衡搜索树实现。适合动态维护“键 → 值”的关系，且需要按键排序或查询前驱、后继的场景。

```cpp
map<string, int> score;

score["Alice"] = 95;
score["Bob"] = 87;
score["Alice"] = 100;  // 已有 key：修改 value

// 按 key 字典序遍历，而不是按成绩或插入顺序
for (const auto& p : score) {
    cout << p.first << ' ' << p.second << '\n';
}
```

- `set<T>` 存储唯一的元素；`map<Key, Value>` 存储唯一 **key** 对应的 **value**，不同 key 可以对应相同 value。
- `map` 保证按 key 有序，增删查通常为 `O(log n)`；`unordered_map` 不保序，增删查平均 `O(1)`、最坏 `O(n)`。

## 2. 常用 API

以 `map<string, int>` 为例，以下为 **C++11 常用接口签名**，省略少见重载及部分 `const` 版本。

其中 `iterator`、`const_iterator`、`size_type` 是容器的成员类型；`value_type` 为 `pair<const string, int>`。

| API 签名 | 作用 / 返回值 | 复杂度 |
|---|---|---|
| `int& operator[](const string& key)` | 返回对应 value 的引用；key 不存在则插入默认值 `0` | `O(log n)` |
| `int& at(const string& key)` | 返回已有 key 对应的 value 引用；不存在则抛出 `out_of_range` | `O(log n)` |
| `pair<iterator, bool> insert(const value_type& value)` | 插入键值对；返回迭代器及是否新插入 | `O(log n)` |
| `template<class... Args> pair<iterator, bool> emplace(Args&&... args)` | 用参数构造键值对并尝试插入；返回位置及成功标记 | `O(log n)` |
| `iterator find(const string& key)` | 查找 key；不存在返回 `end()` | `O(log n)` |
| `size_type count(const string& key) const` | 返回 `0` 或 `1`，表示 key 是否存在 | `O(log n)` |
| `size_type erase(const string& key)` | 按 key 删除，返回删除数量（`0` 或 `1`） | `O(log n)` |
| `iterator erase(const_iterator pos)` | 删除指定元素，返回下一位置 | 均摊 `O(1)` |
| `iterator lower_bound(const string& key)` | 第一个键 `>= key` 的位置 | `O(log n)` |
| `iterator upper_bound(const string& key)` | 第一个键 `> key` 的位置 | `O(log n)` |
| `iterator begin()` / `iterator end()` | 最小 key 的位置 / 尾后位置 | `O(1)` |
| `size_type size() const` / `bool empty() const` | 键值对数量 / 是否为空 | `O(1)` |
| `void clear()` | 清空全部元素 | `O(n)` |

`n` 为键值对数量；表中树操作的 `O(log n)` 以键比较为基本操作。对于 `string` key，一次比较本身还可能读取多个字符。

### 2.1 `operator[]`：访问、修改，也可能插入

```cpp
map<string, int> m;

cout << m["Alice"]; // 0：不存在时自动插入 {"Alice", 0}
cout << m.size();    // 1

m["Alice"] = 90;    // 修改已有 value
++m["Bob"];        // 计数：首次出现时先自动插入 0
```

**不要用 `m[key]` 进行无副作用的存在性查询。** 如果只想查找：

```cpp
auto it = m.find("Alice");

if (it != m.end()) {
    cout << it->first << ' ' << it->second << '\n';
}
```

- `it->first`：key，类型具有 `const` 属性，不能直接修改。
- `it->second`：value，可以修改。
- `end()` 不能解引用。

如果希望“key 不存在时直接报错”，可以使用 `m.at(key)`，它不会自动插入，但不存在时会抛出异常。

### 2.2 `insert()`、`emplace()` 与赋值的区别

```cpp
map<string, int> m;

// 仅当 key 不存在时插入；不覆盖旧值
m.insert(make_pair("Alice", 90));
m.emplace("Alice", 100);   // 不覆盖，Alice 仍是 90

// 返回 pair<iterator, bool>，second 表示是否新插入
// C++11 写法，不使用结构化绑定
auto result = m.emplace("Bob", 85);
if (result.second) {
    cout << result.first->second << '\n';  // 85
}

// 需要新增或覆盖时，直接用 operator[]
m["Alice"] = 100;
```

> `emplace()` 可以直接传入构造键值对的参数，某些情况下能减少不必要的临时对象；但不保证总比 `insert()` 快，也可能在发现重复 key 前先构造元素。

### 2.3 删除返回值与有序查询

```cpp
if (m.erase("Alice")) {
    // 原来存在 Alice，本次成功删除
}

auto it = m.lower_bound("Bob");  // 第一个 key >= "Bob" 的键值对
if (it != m.end()) {
    cout << it->first << '\n';
}
```

需要有序查询时，用**成员函数** `m.lower_bound(key)` / `m.upper_bound(key)`，不要用通用算法在 `map` 的双向迭代器上推进，因为迭代器移动总次数可能达到线性级别。

## 3. 高频陷阱

| 易错点 | 原因 / 正确做法 |
|---|---|
| 只想查询，却使用 `m[key]` | 不存在的 key 会被插入，value 默认初始化 |
| 以为 `insert()` / `emplace()` 会更新已有 key | 它们不会覆盖旧 value；更新用 `m[key] = value` |
| 直接把 `find()` 的结果当作整数 | 它返回迭代器，value 要通过 `it->second` 访问 |
| 想按 value 顺序遍历 | `map` 只按 **key** 排序，不按 value 排序 |
| 认为可以修改 `it->first` | key 不能原地修改；需要删除旧 key 并重新插入 |
| 对 `end()` 解引用 | 先检查 `it != m.end()` |

**插入新元素不使已有迭代器失效；删除操作仅使被删除元素的迭代器失效。**

## 4. 代表题：[P5266 〖深基17.例6〗学籍管理](https://www.luogu.com.cn/problem/P5266)

**题型：键值映射 / 增删改查 / 利用操作返回值。**

维护姓名到成绩的映射：插入或修改成绩、查询成绩、删除学生、统计人数。

### 核心实现

```cpp
void solve() {
    int n;
    cin >> n;

    map<string, int> tab;

    while (n--) {
        int op;
        cin >> op;

        if (op == 4) {
            cout << tab.size() << '\n';
            continue;
        }

        string name;
        cin >> name;

        if (op == 1) {
            int score;
            cin >> score;

            tab[name] = score; // 不存在则插入，存在则更新
            cout << "OK\n";
        }
        else if (op == 2) {
            auto it = tab.find(name); // 查询不应意外插入

            if (it == tab.end())
                cout << "Not found\n";
            else
                cout << it->second << '\n';
        }
        else if (op == 3) {
            // erase(key) 返回删除数量，省去先 find 再 erase
            if (tab.erase(name))
                cout << "Deleted successfully\n";
            else
                cout << "Not found\n";
        }
    }
}
```

这道题的关键不在复杂算法，而在于根据操作语义选择接口：

- **新增或更新**：`operator[]`。
- **只查询**：`find()`，避免缺失 key 自动插入。
- **删除并判断是否存在**：直接使用 `erase(key)` 的返回数量。
- **统计数量**：`size()`。

使用 `map` 时，整体树操作复杂度为 `O(n log n)`（不计字符串自身的比较开销）。

本题**不要求有序输出**，因此用 `unordered_map<string, int>` 也成立，平均树/哈希操作数量可降到 `O(n)`；但哈希操作最坏可能退化至 `O(n²)`。两种容器的接口相近，选型由是否需要有序性与复杂度保证决定。

## 5. 选型触发器

> **需要维护 key → value，并且需要按 key 有序遍历、前驱后继或稳定的对数级增删查 → `map`。**

如果只需要按 key 快速定位 value，不依赖顺序，可以考虑 `unordered_map`。
