# 1.4.4 `deque`：双端队列

## 1. 定位与选型

`deque`（double-ended queue）支持**首尾 O(1) 插入、删除**，同时支持**按下标 O(1) 随机访问**。

```cpp
deque<int> dq = {2, 3, 4};

dq.push_front(1);  // {1, 2, 3, 4}
dq.push_back(5);   // {1, 2, 3, 4, 5}
dq.pop_front();    // {2, 3, 4, 5}
dq.pop_back();     // {2, 3, 4}

cout << dq[1];     // 3
```

与 `vector` 不同，`deque` **不要求元素存放在一整段连续内存里**；中间位置的插入、删除仍可能需要移动元素。

**适用场景：频繁从两端加入、取走元素，同时可能需要下标访问。** 如果只是 FIFO，可优先用接口更受限的 `queue`。

## 2. 常用 API

以下以 `deque<int>` 的非 `const` 对象为例，列出常用重载。`size_type`、`iterator`、`const_iterator` 是容器中的类型别名；省略不常用重载及部分限定符。

| API 签名 | 作用 / 返回值 | 复杂度 |
|---|---|---|
| `void push_front(const int& value)` | 在队首插入元素 | O(1) |
| `void push_back(const int& value)` | 在队尾插入元素 | O(1) |
| `void pop_front()` | 删除队首元素，不返回值 | O(1) |
| `void pop_back()` | 删除队尾元素，不返回值 | O(1) |
| `int& front()` | 返回首元素的引用 | O(1) |
| `int& back()` | 返回尾元素的引用 | O(1) |
| `int& operator[](size_type pos)` | 下标访问；不检查越界 | O(1) |
| `size_type size() const` | 返回元素个数 | O(1) |
| `bool empty() const` | 判断是否为空 | O(1) |
| `iterator begin()` / `iterator end()` | 首元素 / 尾后迭代器 | O(1) |
| `iterator erase(const_iterator pos)` | 删除指定位置，返回下一个元素的迭代器 | 最坏 O(n) |
| `void clear()` | 清空全部元素 | O(n) |

其中 `n` 为当前容器的元素个数。

```cpp
deque<int> dq;

dq.push_back(20);
dq.push_front(10);
dq.push_back(30);

cout << dq.front();  // 10
cout << dq.back();   // 30
cout << dq[1];      // 20

dq.pop_front();     // {20, 30}

for (int x : dq)
    cout << x << ' ';
```

`pop_front()` 和 `pop_back()` 都不返回被删除元素；若需要它的值，先调用 `front()` 或 `back()`。

## 3. 高频陷阱

### 3.1 首尾高效，不代表中间也高效

```cpp
dq.push_front(x);             // O(1)
dq.push_back(x);              // O(1)
dq.erase(dq.begin() + k);     // 最坏 O(n)
```

如果题目要求大量中间位置插入、删除，不能因为 `deque` 有双端操作就认为它们也都是常数时间。

### 3.2 有随机访问，不代表内存连续

```cpp
cout << dq[0];  // 合法：O(1) 随机访问
```

`deque` 通常采用分段存储，不适合依赖整段连续内存的用法；这类需求优先使用 `vector`。

### 3.3 空容器与迭代器失效

```cpp
if (!dq.empty()) {
    int x = dq.front();
    dq.pop_front();
}
```

- 空 `deque` 不能调用 `front()`、`back()`、`pop_front()`、`pop_back()`。
- 首尾插入可能使**所有迭代器**失效；即使旧元素的引用仍有效，也不能继续使用旧迭代器。
- 中间插入、删除可能使大量迭代器和引用失效；修改后应重新取得需要的位置。
- `dq[i]` 不做边界检查，下标必须满足 `0 <= i < dq.size()`。

**对照 `vector`：** 如果大量操作集中在末尾且需要连续内存，优先 `vector`；如果需要真实的双端动态插删，考虑 `deque`。

## 4. 代表题：[CF 381A — Sereja and Dima](https://codeforces.com/problemset/problem/381/A)

**题型：双端取数 / 顺序模拟**

给定一排牌，两人轮流从**最左或最右**取一张牌，每轮必须拿当前两端数值较大的那张，求双方总分。题目规定了取牌策略，不需要另外证明贪心最优。

### 4.1 独立实现：`deque` 直接模拟

你的 AC 核心代码：

```cpp
deque<int> dq;

int t;
while (cin >> t)
    dq.push_back(t);

int a = 0, b = 0;

for (int i = 1; i <= n; ++i) {
    int add = 0;

    if (dq.front() > dq.back()) {
        add = dq.front();
        dq.pop_front();
    } else {
        add = dq.back();
        dq.pop_back();
    }

    if (i & 1) a += add;
    else b += add;
}
```

**算法正确。** 双端比较与删除都由 `deque` 的接口直接表达；通过轮数奇偶区分两名玩家。

唯一需要改进的是**输入范围**：原题已给出 `n`，应使用 `for` 循环恰好读取 `n` 个元素。`while (cin >> t)` 在本题单组输入中没有问题，但在后续还有其他输入数据（如多组测试）时，可能误读后续数据；按 `n` 读取更符合题意，也更稳健。

```cpp
// 将 while (cin >> t) 替换为恰好读取 n 个元素
for (int i = 0; i < n; ++i) {
    int x;
    cin >> x;
    dq.push_back(x);
}
```

这不影响原题单组输入下的正确性，但按题意限定输入数量更稳妥。

### 4.2 进一步观察：没有必要真的删除元素

本题每轮只会从**原序列左右边缘**取走一张牌，被取走的牌永远不再参与运算。

因此可以直接维护有效区间 `[l, r]`，用 `vector + 双指针` 代替真实删除：

```cpp
vector<int> v(n);
for (int& x : v) cin >> x;

int l = 0, r = n - 1;
int a = 0, b = 0;

for (int i = 0; i < n; ++i) {
    int x;

    if (v[l] > v[r]) x = v[l++];
    else x = v[r--];

    if (i % 2 == 0) a += x;
    else b += x;
}

cout << a << ' ' << b << '\n';
```

| 解法 | 时间 | 空间 | 核心做法 |
|---|---|---|---|
| `deque` | O(n) | O(n) | 真实删除左右端元素 |
| `vector + 双指针` | O(n) | O(n) | 保留原数组，只收缩有效区间 |

两种方案都满足要求。`deque` 更直接体现双端队列操作；本题的 `vector + 双指针` 更轻量。

> **如果删除只是表示某些元素以后不再参与计算，可以考虑移动有效区间边界，而不是真正从容器中删除。**

### 4.3 思维提炼

- 规则要求从两端取数 → `deque` 的 `front/back + pop_front/pop_back` 能直接模拟。
- 数据已有固定顺序、取走后不会恢复 → 尝试 `vector + l/r` 两个下标。
- 已知输入数量 `n` → 读取恰好 `n` 个元素，不用 `while (cin >> x)` 读取到 EOF。

## 5. 选型触发器

> **频繁双端插入 / 删除 + 仍需随机访问 → 考虑 `deque`。**

> **只是顺序处理先进先出 → 考虑 `queue`；只是两端收缩有效区间 → 不必真的删除，可用 `vector + 双指针`。**

**一句话：`deque` 擅长双端动态操作，但题目里的“取走”不一定意味着必须调用容器的删除接口。**
