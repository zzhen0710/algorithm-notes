# 1.4.16 `tuple`：多元组与多关键字排序

## 1. 核心定位

`tuple<Ts...>` 是 C++11 提供的**固定长度、可包含不同类型成员的工具类型**，相当于把 `pair` 的两个成员扩展为任意多个成员。它不是支持运行时增删元素的容器。

```cpp
tuple<int> one{7};
tuple<int, int, int> point{1, 2, 3};
tuple<int, string, double, char> record{1, "Alice", 95.5, 'A'};

cout << get<0>(point) << ' ' << get<2>(point); // 1 3
get<1>(point) = 10;                          // 修改第二个成员
```

元组的成员**数量与类型在编译期固定**，`get<I>()` 的下标 `I` 也必须是编译期常量。元素数量并不限于三个：一元组、四元组乃至 `tuple<>` 都是合法类型。

**核心价值**：把多个属性绑定成一条记录，并利用 `tuple` 自带的**字典序比较**完成多关键字排序。

## 2. 常用接口、返回类型与复杂度

下面以 `tuple<int, int, int>` 为主要示例，列出 C++11 常用形式。构造函数没有返回类型，表中的函数模板签名为便于学习的常用形式，省略部分重载。

| 写法 / API 签名 | 返回类型 / 作用 | 复杂度 |
|---|---|---|
| `tuple<int, int, int> t{a, b, c}` | 构造三个整数字段的元组 | `O(1)` |
| `make_tuple(a, b, c)` | 若参数均为 `int`，返回 `tuple<int, int, int>` | `O(1)` |
| `int& get<0>(tuple<int, int, int>& t)` | 返回第一个成员的可修改引用；`get<1>`、`get<2>` 类似 | `O(1)` |
| `const int& get<0>(const tuple<int, int, int>& t)` | 返回第一个成员的只读引用 | `O(1)` |
| `tuple<Ts&...> tie(Ts&... args)` | 将若干左值引用打包，常用于接收元组成员 | `O(1)` |
| `bool operator==(const tuple<int, int, int>& x, const tuple<int, int, int>& y)` | 所有对应成员均相等时返回 `true` | `O(1)` |
| `bool operator<(const tuple<int, int, int>& x, const tuple<int, int, int>& y)` | 按字段顺序进行字典序比较 | `O(1)` |
| `void swap(tuple<int, int, int>& x, tuple<int, int, int>& y)` | 交换两个元组的对应成员 | `O(1)` |
| `tuple_size<tuple<int, int, int>>::value` | 编译期常量，值为 `3`，类型为 `size_t` | `O(1)` |

表中构造、比较与交换的复杂度针对**三个整数成员**。如果有更多成员，比较最多检查全部字段；若成员是字符串等类型，还需要计入其自身操作成本。

`make_tuple()` 会根据实参推导成员类型（通常进行类型退化），而非一律保持实参的引用属性。

### 2.1 `get<I>()`：编译期下标，不是运行时数组访问

```cpp
tuple<int, string, int> t{1, "Alice", 95};

cout << get<0>(t);  // 1
cout << get<1>(t);  // Alice
get<2>(t) = 100;

// int i = 2;
// get<i>(t); // 错误：i 不是编译期常量
```

对于非 `const` 左值元组，`get<I>(t)` 返回对应成员的引用；如果元组是 `const`，则返回只读引用。`get<0>` 里的 `0` 表示**字段编号从 0 开始**。

### 2.2 `tie()`：把已有变量绑定为引用元组

```cpp
tuple<int, int, int> t{240, 90, 7};
int total, chinese, id;

tie(total, chinese, id) = t;
cout << total << ' ' << chinese << ' ' << id; // 240 90 7
```

`tie(total, chinese, id)` 返回 `tuple<int&, int&, int&>`，并不是复制生成一个全新的数据记录。执行赋值时，右侧 `t` 的三个值被赋给这些引用绑定的变量；**不修改 `t`**。

不需要某个字段时，可以用 `ignore`：

```cpp
int total, id;
tie(total, ignore, id) = t; // 跳过第二个字段
```

C++11 没有结构化绑定；`auto [x, y, z] = t;` 是 **C++17** 的语法，不能用于限定 C++11 的竞赛代码。

### 2.3 默认字典序与排序

```cpp
tuple<int, int, int> a{2, 5, 100};
tuple<int, int, int> b{2, 8, 1};
cout << (a < b); // 1：第一个字段相同，第二个字段 5 < 8

vector<tuple<int, int, int>> v;
v.emplace_back(3, 5, 2);
v.emplace_back(1, 9, 3);
v.emplace_back(3, 2, 1);
sort(v.begin(), v.end());
// 排序结果：{1,9,3}, {3,2,1}, {3,5,2}
```

`tuple` 比较会**从左到右依次检查字段**，直到遇到不相等的字段；默认升序因此等价于第一关键字升序、平手看第二关键字、再平手看第三关键字……

`vector<tuple<...>>::emplace_back(Args&&... args)` 会直接在末尾构造元组，**C++11 返回 `void`**，均摊时间复杂度为 `O(1)`（扩容时单次可达到 `O(n)`）。

## 3. 代表题：[P1093 奖学金](https://www.luogu.com.cn/problem/P1093)

**题型**：多关键字排序 / 自定义比较器 / 元组解包 / 数据表示转换。

给定学生的语文、数学、英语成绩，按以下优先级排名，输出前五名的**学号与总分**：

1. 总分**降序**。
2. 总分相同时，语文成绩**降序**。
3. 前两项都相同时，学号**升序**。

关键在于明确每条记录真正参与排序的字段：

```cpp
// {总分, 语文成绩, 学号}
a.emplace_back(chinese + math + english, chinese, id);
```

数学、英语成绩只参与计算总分，**不需要单独占据排序字段**。

### 3.1 方案一：自定义比较 + `tie()` 解包

通过比较器明确表达三个关键字的优先级；只有前一关键字相同时才比较下一关键字。

```cpp
bool rule(const tuple<int, int, int>& x,
          const tuple<int, int, int>& y) {
    int a, b, c, i, j, k;
    tie(a, b, c) = x;
    tie(i, j, k) = y;

    if (a != i) return a > i; // 总分降序
    if (b != j) return b > j; // 语文成绩降序
    return c < k;             // 学号升序
}

void solve() {
    int n;
    cin >> n;

    vector<tuple<int, int, int>> a;
    a.reserve(n);

    for (int i = 1; i <= n; ++i) {
        int x, y, z;
        cin >> x >> y >> z;
        a.emplace_back(x + y + z, x, i);
    }

    sort(a.begin(), a.end(), rule);

    for (int i = 0; i < 5; ++i) {
        const auto& v = a[i];
        cout << get<2>(v) << ' ' << get<0>(v) << '\n';
    }
}
```

- `rule(x, y)` 返回 `bool`，`true` 表示 `x` 应排在 `y` 前面。
- `const tuple<...>&` 避免传参时复制整条记录；函数内部 `tie()` 解包三个整数时有少量常数次复制，不影响渐近复杂度。
- 三个关键字完全相同时，比较器返回 `false`，符合 `sort()` 的**严格弱序**要求。比较器中应使用严格的 `<` 或 `>`，不能把相等元素也判定为彼此排在前面。
- 题目保证至少五名学生，输出前五名不越界。

### 3.2 方案二：改变数据表示，直接使用默认排序

> 默认的 `tuple` 字典序是逐字段**升序**。但题目要求“总分降序、语文降序、学号升序”，可以将前两项变为其相反数：

```cpp
// 原始排序要求：总分降序、语文降序、学号升序
// 转换后存储：{-总分, -语文成绩, 学号}
vector<tuple<int, int, int>> a;

for (int i = 1; i <= n; ++i) {
    int x, y, z;
    cin >> x >> y >> z;
    a.emplace_back(-(x + y + z), -x, i);
}

sort(a.begin(), a.end()); // 直接使用默认字典序升序

for (int i = 0; i < 5; ++i) {
    cout << get<2>(a[i]) << ' ' << -get<0>(a[i]) << '\n';
}
```

原始成绩均非负且范围有限，这里取负安全。对于一般整数，若可能出现 `INT_MIN`，直接取负会发生溢出，不能机械套用此技巧。

**两种方案的比较**：

| 方案 | 核心做法 | 适用场景 |
|---|---|---|
| 自定义比较 | 直接逐项表达升序 / 降序与优先级 | 规则复杂、需要清楚体现题意、可能不能安全取负 |
| 转换字段后默认排序 | 用相反数将降序统一成升序 | 简单整数关键字、取负安全、希望省去比较函数 |

两种方案都只排序一次，并没有渐近性能上的差异。核心是**明确优先级**，而不是执着于哪种写法更短。

### 3.3 复杂度与思维总结

设学生数为 `n`，比较固定三个整数的代价为 `O(1)`：

- 构造记录 `O(n)`；
- 排序 `O(n log n)`；
- 输出五名 `O(1)`；
- 总时间 `O(n log n)`，保存元组所需空间 `O(n)`。

> **多关键字排序：先提取真正决定次序的字段，明确优先级与方向；再选择用比较器直接描述规则，或改变字段表示以复用默认字典序。**

## 4. 易错点

| 常见错误 | 原因与修正 |
|---|---|
| 认为 `tuple` 只能存三个成员 | `tuple<Ts...>` 的字段数量可为 0、1、2、3……，编译期固定 |
| 使用 `t.first` / `t.second` | `tuple` 用 `get<I>(t)` 访问，不提供 `pair` 的两个命名成员 |
| 用运行时变量作为 `get<i>(t)` 的下标 | `get<I>` 的字段编号必须是编译期常量 |
| 使用 C++17 结构化绑定来写 C++11 题目 | C++11 改用 `tie()` 或 `get<I>()` |
| 比较器在关键字相等时返回 `true` | `sort` 要求严格弱序；完全相等时应返回 `false` |
| 把总分、语文等全部成绩当作三个原始学科字段存入元组 | P1093 排序所需的是 `{总分, 语文, 学号}`，数学和英语只用来计算总分 |
| 以为 `tie()` 会修改右侧元组 | `tie()` 绑定左侧变量，赋值操作从右侧元组读取成员 |

## 5. 选型触发器

> **固定数量的多种属性需要绑定、返回或按多个关键字排序 → `tuple`；只有两个属性时用 `pair` 通常更简洁。**

字段较多、含义复杂、需要频繁修改时，使用有字段名的 `struct` 通常更易读；`tuple` 的优势是紧凑、便于组合和默认字典序比较。
