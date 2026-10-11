# 1.4.7 `priority_queue`：优先队列（堆）

## 1. 核心定位

`priority_queue` 是按**优先级**取出元素的容器适配器，默认是**大根堆**：每次 `top()` 得到当前最大值，而非最先插入的值。

```cpp
priority_queue<int> mx;                             // 大根堆
priority_queue<int, vector<int>, greater<int>> mn;  // 小根堆

mn.push(5);
mn.push(2);
mn.push(8);
cout << mn.top();  // 2
mn.pop();          // 删除 2，此后堆顶为 5
```

**典型场景**：动态插入元素，反复取出当前最大 / 最小值；任务优先级调度、贪心合并、最短路等。

底层通常采用堆结构，默认底层容器为 `vector`。**只保证堆顶符合优先级，不保证其余元素整体有序**；允许重复元素。

---

## 2. 常用 API

以下以 `priority_queue<int>` 为例，列出常用的 **C++11 接口签名**（省略部分重载）。

| API 签名 | 作用与返回值 | 时间复杂度 |
|---|---|---|
| `void push(const int& value)` | 插入元素，不返回值 | \(O(\log n)\) |
| `void push(int&& value)` | 插入右值，不返回值 | \(O(\log n)\) |
| `void pop()` | 删除堆顶，**不返回被删除的值** | \(O(\log n)\) |
| `const int& top() const` | 返回堆顶元素的只读引用 | \(O(1)\) |
| `size_type size() const` | 返回元素个数 | \(O(1)\) |
| `bool empty() const` | 判断是否为空 | \(O(1)\) |
| `template<class... Args> void emplace(Args&&... args)` | 在容器中构造并插入元素 | \(O(\log n)\) |

`size_type` 为容器定义的无符号大小类型；复杂度以一次比较为常数时间为前提。

```cpp
priority_queue<int> pq;
pq.push(7);
pq.push(4);

if (!pq.empty()) {
    int x = pq.top(); // 7
    pq.pop();          // 先取值，再删除
}
```

`priority_queue` 不提供 `operator[]`、`begin()` / `end()`、`erase()` 或 `clear()`；不能直接定位并删除任意指定元素。

---

## 3. 大根堆、小根堆与比较器

### 3.1 两种常用声明

```cpp
priority_queue<int> mx;                             // 默认：最大值优先
priority_queue<int, vector<int>, greater<int>> mn;  // 最小值优先
```

也可通过 `pair` 维护“优先级 + 额外信息”：

```cpp
using pii = pair<int, int>;
priority_queue<pii, vector<pii>, greater<pii>> pq;

pq.push({2, 5});
pq.push({1, 9});
pq.push({1, 3});

// 弹出顺序：(1,3) → (1,9) → (2,5)
```

`pair` 按**字典序**比较：先比较 `first`；相等时再比较 `second`。可用于 `{距离, 节点}`、`{时间, 编号}` 等组合状态。

### 3.2 自定义比较器：最值得记住的方向

```cpp
struct Task {
    int id;
    int priority;
};

struct Cmp {
    bool operator()(const Task& a, const Task& b) const {
        return a.priority < b.priority;
    }
};

priority_queue<Task, vector<Task>, Cmp> pq;
```

> **比较器对 `(a, b)` 返回 `true`，表示 `a` 的优先级低于 `b`，应由 `b` 更先出堆。**

因此，返回 `a.priority < b.priority` 时，`priority` 大的任务优先。若优先级相同还有额外的先后规则，需要在比较器中继续指定；不能假设它会保持插入顺序。

---

## 4. 高频陷阱与选型

| 易错点 | 正确理解 |
|---|---|
| 以为 `top()` 是最小值 | 默认大根堆；需要小根堆时使用 `greater<T>` |
| 以为 `pop()` 是 \(O(1)\) | `top()` 为 \(O(1)\)，`pop()` 为 \(O(\log n)\) |
| `int x = pq.pop();` | 错误：`pop()` 返回 `void`；先 `top()` 再 `pop()` |
| 对空堆访问 `top()` / `pop()` | 不合法；先判断 `empty()` |
| 认为堆内元素整体排序 | 只保证堆顶；不能直接按下标访问或遍历 |
| 需要删除指定的非堆顶元素 | `priority_queue` 不支持直接 `erase(x)`；可考虑 `multiset` |

**选型对照**：

- 只需反复取出最大 / 最小值，且动态加入新元素 → `priority_queue`。
- 要保留重复值、按值删除一个元素、查询前驱后继 → `multiset`。
- 只需维护 FIFO 顺序，不考虑元素大小 → `queue`。

---

## 5. 代表题：[P1090 [NOIP 2004 提高组] 合并果子](https://www.luogu.com.cn/problem/P1090)

**题型**：小根堆 / 贪心 / 哈夫曼最优合并。

给定若干堆果子。每次合并两堆，代价为两堆果子数量之和；合并出的新堆可以继续参与合并。求合并到只剩一堆的最小总代价。

### 5.1 核心难点：合并顺序影响总代价

例如三堆重量为 `1、2、9`：

```text
先合并 1 + 2 = 3，再合并 3 + 9 = 12 → 总费用 15
先合并 2 + 9 = 11，再合并 11 + 1 = 12 → 总费用 23
```

因此每次合并**当前最小的两堆**，并把新堆重新加入小根堆：

```text
取出两个最小值
→ 累加本次合并费用
→ 将合并结果重新入堆
→ 直到只剩一堆
```

### 5.2 独立实现的核心代码

```cpp
priority_queue<int, vector<int>, greater<int>> pq;

// 读入 n 堆果子并逐个 pq.push(x)

int weigh = 0;
while (true) {
    if (pq.size() == 1) break;

    int a = pq.top(); pq.pop();
    int b = pq.top(); pq.pop();

    pq.push(a + b);
    weigh += a + b;
}
```

**原解正确，复杂度为 \(O(n\log n)\)。** 原题对结果范围有保证，使用 `int` 可以通过；做同类问题仍应根据数据范围判断累计答案是否需要 `long long`。

仅需把终止条件写得更直接：

```cpp
while (pq.size() > 1) {
    // 其余合并逻辑不变
}
```

没有必要重复给出另一份几乎相同的完整代码。

### 5.3 为什么“每次取最小两项”保证最优？

这里的正确性来自**哈夫曼贪心**，而不只是“当前合并花费最少”的直觉。

把合并过程表示成二叉树：每个原始果子堆是一个叶子；每次合并生成一个内部节点，其权值等于两个子节点的权值之和。每个叶子的重量，会在其到根路径上的每次合并中被计费一次。

因此总费用等于加权路径长度（WPL）：

\[
\text{cost}=\sum_i w_i\cdot depth_i
\]

- 权值越小，放在越深的位置越划算。
- 在一棵最优合并树中，可以使**最小的两项成为最深的一对兄弟叶子**，而不增加总费用。
- 因此可以先合并它们，用权值 `a + b` 的新节点代替，再解决规模减一的同类问题。

> 这同时说明了哈夫曼编码与合并果子的联系：前者用**字符频率**作权值、最小化总编码位数；后者用**果子数量**作权值、最小化总合并费用。两者最小化的是同一个 WPL 目标。

> **`priority_queue` 负责高效找出最小两项；贪心证明负责说明为什么这样选能得到全局最优。**

---

## 6. 选型触发器

```text
不断加入新元素
+
每次只需要取当前最大 / 最小元素
+
不需要删除任意指定的非堆顶元素
→ 优先考虑 priority_queue
```

**一句话总结：`priority_queue` 维护的是“下一次该取谁”，哈夫曼贪心解决的是“为什么应该取这两个”。**
