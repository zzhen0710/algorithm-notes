# 1.4.5 `stack`：后进先出的栈

## 1. 核心定位

`stack` 是 **LIFO（Last In, First Out，后进先出）** 的容器适配器：只能从栈顶压入、访问和弹出元素。

```cpp
stack<int> st;

st.push(10);
st.push(20);
st.push(30);

cout << st.top(); // 30
st.pop();
cout << st.top(); // 20
```

**典型场景**：括号匹配、表达式求值、撤销操作、维护“最近一个尚未处理”的对象。

默认底层容器是 `deque`；`stack` 只是限制了可用接口，并非一种必须拥有独立存储结构的容器。

---

## 2. 常用 API

以下以 `stack<int>` 为例，列出常用的 **C++11 接口形式**，省略部分不常用重载。

| API 签名 | 作用与返回值 | 时间复杂度 |
|---|---|---|
| `void push(const int& value)` | 把元素压入栈顶，无返回值 | O(1) |
| `void pop()` | 删除栈顶元素，**不返回被删除的值** | O(1) |
| `int& top()` | 返回栈顶元素的引用 | O(1) |
| `const int& top() const` | 只读访问栈顶元素 | O(1) |
| `size_type size() const` | 返回当前元素数量 | O(1) |
| `bool empty() const` | 判断是否为空 | O(1) |

以上复杂度针对默认的 `deque` 底层容器。

### 基本调用

```cpp
stack<int> st;

st.push(7);
st.push(9);

int x = st.top(); // 9
st.pop();         // 栈中只剩 7

if (!st.empty()) {
    cout << st.top();
}
```

`stack` **没有** `operator[]`、`begin()`、`end()` 等随机访问或迭代接口。

---

## 3. 高频陷阱

### 3.1 `pop()` 只删除，不返回值

```cpp
int x = st.pop(); // 错误：pop() 返回 void
```

正确：

```cpp
int x = st.top();
st.pop();
```

若需要访问弹出的元素，必须在 `pop()` 之前读取；`pop()` 后不要继续使用指向原栈顶元素的引用。

### 3.2 空栈不能 `top()` 或 `pop()`

```cpp
if (!st.empty()) {
    int x = st.top();
    st.pop();
}
```

题目若保证输入合法、运算符一定有足够的操作数，可直接按题意处理，不必机械增加检查。

### 3.3 弹出顺序不是运算顺序

例如后缀表达式 `8 3 -`：

```cpp
int b = st.top(); st.pop(); // 右操作数 3
int a = st.top(); st.pop(); // 左操作数 8

st.push(a - b);              // 5，不能写成 b - a
```

加法、乘法可交换；**减法和除法必须保留左右操作数顺序**。

---

## 4. 代表题：[P1449 后缀表达式](https://www.luogu.com.cn/problem/P1449)

**题型**：栈 / 字符串解析 / 表达式求值。

题目将数字写在 `.` 之前，用 `+ - * /` 表示运算，以 `@` 结束整个表达式。

例如：

```text
3.5.2.-*7.+@
```

对应 `(3 * (5 - 2)) + 7`，答案是 `16`。

### 4.1 核心建模

```text
读取数字 → 整体压栈
遇到运算符 → 弹出右操作数、左操作数 → 计算 → 结果压栈
遇到 @ → 结束，栈顶为答案
```

### 4.2 独立实现的关键部分

原实现先收集数字字符，再用 `stoi()` 转成整数：

```cpp
string t;

for (char c : s) {
    if (c == '@') break;

    if (c == '.') {
        stk.push(stoi(t));
        t.clear();
        continue;
    }

    if (c >= '0' && c <= '9') {
        t += c;
    } else {
        int b = stk.top(); stk.pop();
        int a = stk.top(); stk.pop();

        if (c == '+') stk.push(a + b);
        else if (c == '-') stk.push(a - b);
        else if (c == '*') stk.push(a * b);
        else stk.push(a / b);
    }
}
```

**正确之处**：

- `.` 作为多位整数的结束标记，不会把 `123.` 误拆成三个数；
- 使用 `top()` 后立即 `pop()`，并正确区分左、右操作数；
- 直接按合法输入规则计算，不添加无必要的状态。

### 4.3 实现改进：直接累积整数

这道题只需处理非负整数，可以省去临时字符串和 `stoi()`：

```cpp
int num = 0;

for (char c : s) {
    if (c >= '0' && c <= '9') {
        num = num * 10 + (c - '0');
    }
    else if (c == '.') {
        stk.push(num);
        num = 0;
    }
    else if (c == '@') {
        break;
    }
    else {
        int b = stk.top(); stk.pop();
        int a = stk.top(); stk.pop();

        if (c == '+') stk.push(a + b);
        else if (c == '-') stk.push(a - b);
        else if (c == '*') stk.push(a * b);
        else stk.push(a / b);
    }
}
```

数字累积规则：

\[
num_{new}=num_{old}\times10+(c-'0')
\]

例如 `123`：`0 → 1 → 12 → 123`。

**性质**：两种方法都是线性扫描，渐近时间复杂度均为 O(n)，其中 n 是表达式长度。直接累积整数只是省去中间字符串及转换步骤，**不是算法复杂度层面的提升**。

---

## 5. 选型触发器

当题目出现以下结构，优先想到 `stack`：

```text
最近加入的对象最先处理
最后执行的操作最先撤销
遇到结束标志后回收先前保存的状态
运算符依赖最近两个操作数
```

> **栈的关键不是“存放数据”，而是用后进先出的顺序组织待处理的信息。**

如果需求是“最早加入的先处理”，考虑 `queue`；如果需要在两端修改或随机访问，考虑 `deque`。
