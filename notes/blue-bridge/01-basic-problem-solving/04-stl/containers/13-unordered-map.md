# 1.4.13 `unordered_map`：哈希键值映射

## 1. 核心定位

`unordered_map<Key, Value>` 以哈希表维护 **key 唯一的「key → value」映射**，不保证遍历顺序。适合频率统计、按标识符快速查找信息、为每个对象维护独立状态等场景。

```cpp
unordered_map<string, int> cnt;

++cnt["apple"];
++cnt["banana"];
++cnt["apple"];

cout << cnt["apple"];  // 2
```

| 容器 | 存储内容 | 顺序性质 | 增删查的典型复杂度 |
|---|---|---|---|
| `set<T>` | 唯一 key | 按 key 有序 | `O(log n)` |
| `unordered_set<T>` | 唯一 key | 无序 | 平均 `O(1)`，最坏 `O(n)` |
| `map<Key, Value>` | 唯一 key 对应 value | 按 key 有序 | `O(log n)` |
| `unordered_map<Key, Value>` | 唯一 key 对应 value | 无序 | 平均 `O(1)`，最坏 `O(n)` |

**`unordered_set` 只回答「有没有这个 key」，`unordered_map` 还可以回答「这个 key 对应什么信息」。** 映射中的不同 key 可以拥有相同的 value。

## 2. 常用 API

以 `unordered_map<string, int>` 为例，以下是 **C++11 常用接口签名**；省略少见重载及部分 `const` 版本。`iterator`、`const_iterator`、`size_type` 均为容器成员类型；`value_type` 即 `pair<const string, int>`。

| API 签名 | 作用 / 返回值 | 平均复杂度 | 最坏复杂度 |
|---|---|---|---|
| `int& operator[](const string& key)` | 返回 value 引用；key 不存在时插入默认值 `0` | `O(1)` | `O(n)` |
| `int& at(const string& key)` | 返回已有 key 的 value 引用；不存在则抛异常 | `O(1)` | `O(n)` |
| `pair<iterator, bool> insert(const value_type& value)` | 尝试插入键值对，返回位置及是否新插入 | `O(1)` | `O(n)` |
| `template<class... Args> pair<iterator, bool> emplace(Args&&... args)` | 用参数构造键值对并尝试插入 | `O(1)` | `O(n)` |
| `iterator find(const string& key)` | 查找 key；不存在返回 `end()` | `O(1)` | `O(n)` |
| `size_type count(const string& key) const` | 返回 `0` 或 `1`，判断 key 是否存在 | `O(1)` | `O(n)` |
| `size_type erase(const string& key)` | 按 key 删除，返回删除数量 `0` 或 `1` | `O(1)` | `O(n)` |
| `iterator erase(const_iterator pos)` | 删除指定元素，返回下一迭代器 | `O(1)` | `O(n)` |
| `iterator begin()` / `iterator end()` | 起始位置 / 尾后位置；**不表示大小顺序** | `O(1)` | `O(1)` |
| `size_type size() const` / `bool empty() const` | 键值对数量 / 是否为空 | `O(1)` | `O(1)` |
| `void clear()` | 删除所有元素 | `O(n)` | `O(n)` |
| `void reserve(size_type count)` | 为预计元素数量提前准备桶空间 | `O(n)` | `O(n²)` |

表中的 `n` 表示容器内元素数，哈希操作按通常的摊还分析计。哈希冲突或重哈希可能导致最坏复杂度退化；对于字符串 key，**计算哈希及比较字符串本身还涉及字符长度**，不能机械地把全部运行时间都写成严格 `O(n)`。

### 2.1 `operator[]`：缺失时会插入

```cpp
unordered_map<string, int> cnt;

cout << cnt["cat"];  // 0，同时插入 {"cat", 0}
cout << cnt.size();    // 1

++cnt["cat"];        // 1：适合频率统计
```

**不要把 `m[key]` 当作无副作用的存在性查询。** 仅需查询时可以使用：

```cpp
auto it = cnt.find("dog");
if (it != cnt.end()) {
    cout << it->first << ' ' << it->second << '\n';
}
```

- `it->first` 是 key，不能通过迭代器修改；`it->second` 是可修改的 value。
- `at(key)` 不会创建新 key，但缺失时会抛出 `out_of_range`。
- `end()` 不能解引用。

### 2.2 `insert()`、`emplace()` 与直接赋值

```cpp
unordered_map<string, int> m;

m.insert({"Alice", 90});
m.emplace("Alice", 100);  // key 已存在，不覆盖：仍然是 90

auto result = m.emplace("Bob", 85);
if (result.second) {
    cout << result.first->second << '\n';  // 85
}

m["Alice"] = 100;        // 需要覆盖旧 value 时，直接赋值
```

`insert()` / `emplace()` 都返回 `pair<iterator, bool>`；`second` 表示有没有插入**新 key**。`emplace()` 可以利用参数直接构造键值对，有时能减少临时对象，但不保证一定更快，也可能在发现重复 key 前先构造对象。

### 2.3 无序性与迭代器失效

`unordered_map` 没有 `lower_bound()`、`upper_bound()`，也不能通过 `begin()` 获取最小 key。需要键的大小顺序、前驱或后继时，应使用 `map`。

插入可能触发 `rehash`，导致原有**迭代器失效**；`reserve()` / `rehash()` 也可能触发这种变化。重哈希不会使存活元素的引用和指针失效，但不能继续使用之前失效的迭代器。`erase(it)` 只让被删元素的迭代器失效。

## 3. 代表题：[CF 4C — Registration System](https://codeforces.com/problemset/problem/4/C)

**题型**：字符串注册判重 / 哈希映射 / 频率状态 / 利用输入约束消除重复搜索。

系统依次处理注册请求：

- 某用户名首次出现，输出 `OK`，直接注册。
- 若用户名已存在，尝试在其后追加正整数后缀，选取可用的新用户名，输出并注册。

**关键条件：输入的原始用户名只含小写英文字母，而系统生成的新名字带有数字。** 这一条件使原始输入名字与生成名字属于两个不会混淆的类别。

### 3.1 第一种实现：`unordered_set` 保存全部已注册名称

直观方法是保存每一个实际注册成功的用户名，对重复请求从后缀 `1` 开始逐一尝试：

```cpp
unordered_set<string> used;

while (n--) {
    string s;
    cin >> s;

    string t = s;
    bool repeated = false;

    for (int i = 1; !used.insert(t).second; ++i) {
        repeated = true;
        t = s + to_string(i);
    }

    cout << (repeated ? t : "OK") << '\n';
}
```

这份逻辑**正确**：`insert(t).second` 只有在名字尚未使用时才为 `true`，而且生成的新名字也确实插入了集合。

问题不在哈希集合的查询速度，而在于**每次重复注册都从 `1` 重新试探**。例如连续注册 `aaa` 共 `k` 次，候选尝试次数依次约为 `1, 2, 3, ..., k`：

\[
1+2+\cdots+k=\frac{k(k+1)}2=O(k^2).
\]

即使每次哈希操作平均 `O(1)`，总体仍可能进行 `O(n²)` 次哈希操作，出现 TLE。**单次查询快，不代表重复查询总成本低。**

### 3.2 观察：重复请求需要的不是全部名称，而是下一个编号

对某个原始名字 `s`，第一次输出 `OK`，第二次生成 `s1`，第三次生成 `s2`……

由于后续输入也只能是纯字母，**不可能输入 `s1` 这种系统生成的名字**。不同纯字母前缀生成的名称也不会相同。因此只需记住 `s` 已经收到几次注册请求，就能直接算出下一次的后缀编号；无需从 `1` 重新检查，更不必把生成名全部存入集合。

状态由：

```text
unordered_set：已用过的全部用户名
```

改为：

```text
unordered_map：原始用户名 → 已出现次数 / 下一个可用后缀
```

> 这一优化最初受到**空间换时间**思想的启发：**尝试记录额外信息，避免反复搜索**。

但结合题目约束后，最终方案并非简单地增加空间换取时间，而是**改变状态表示，只维护足以决定下一步结果的必要信息**，从而**既消除了重复搜索，也减少了冗余存储**。事实上，映射只保存原始名字和计数；当同一个名字反复注册时，通常比保存全部生成名字更节省存储内容（实际内存还受容器开销影响）。

### 3.3 推荐实现：一次读取、一次映射更新

```cpp
void solve() {
    int n;
    cin >> n;

    unordered_map<string, int> tab;

    while (n--) {
        string s;
        cin >> s;

        // 不存在时自动插入 0；旧值就是本次应使用的编号
        int no = tab[s]++;

        if (no == 0) {
            cout << "OK\n";
        } else {
            cout << s << no << '\n';
        }
    }
}
```

这里 `tab[s]++` 是**后置自增**：先返回旧值，再把计数加一。因此第一次取得 `0`，之后依次取得 `1, 2, 3, ...`，恰好对应题目的注册规则。

与先 `int no = tab[s];`、再 `++tab[s];` 相比，这种写法减少了对相同 key 的重复查找；两者都正确。

**复杂度**：`n` 次请求，每次平均进行常数次哈希映射操作，故平均为 `O(n)` 次容器操作；在极端哈希冲突下理论最坏可达 `O(n²)`。加上字符串读入、哈希和输出，还需考虑用户名长度及数字后缀长度。空间为 `O(d)` 个键值对（另计字符串），其中 `d` 是不同原始用户名的数量，`d ≤ n`。

### 3.4 正确性依赖题目条件

如果题目允许用户直接输入带数字的名字，例如先注册 `abc`、再次注册 `abc` 生成 `abc1`，随后又输入 `abc1`，那么只统计纯粹的原始前缀就不一定正确；必须额外判断已经生成的名称是否被占用。

**优化不能脱离数据约束。** 本题允许直接计算下一个编号，恰恰因为输入保证了“原始名称”和“生成名称”不会冲突。

## 4. 高频错误速查

| 易错点 | 原因 | 正确做法 |
|---|---|---|
| 认为 `unordered_map` 可以按 key 顺序遍历 | 哈希表不保证元素顺序 | 需要有序查询时选择 `map` |
| 用 `m[key]` 判断是否存在，却不想插入 | 缺失 key 会被自动插入默认值 | 用 `find()` 或 `count()` |
| 用 `emplace(key, value)` 期待覆盖旧值 | key 已存在时不会替换对应 value | 使用 `m[key] = value` |
| 保存迭代器后持续插入，仍直接使用旧迭代器 | 插入可能触发重哈希 | 需要时重新 `find()` |
| 认为哈希查询 `O(1)` 就能避免 TLE | 可能重复执行了二次数量级的查询 | 统计**总操作次数**，保存下一步所需状态 |
| 忽略用户名字符限制，直接计算后缀 | 如果后续可输入生成名，可能发生名称冲突 | 先验证输入约束，再证明优化有效 |

## 5. 选型触发器

> **需要按 key 高效查询或更新某个 value，不需要按 key 排序 → `unordered_map`。**

> **重复工作来自每次重新寻找答案时，考虑维护能直接推导下一次答案的状态（如计数器、下一个位置、下一个编号）。**

CF 4C 的核心收获不只是从 `unordered_set` 换成 `unordered_map`，而是从“保存所有已经使用的结果并反复试探”升级为“利用输入性质，直接维护生成下一个结果所需的最小状态”。
