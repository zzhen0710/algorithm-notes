# 1.4.3 `string`：字符串容器

## 1. 核心定位

`std::string` 是可动态增长的字符序列，支持下标访问、尾部追加、子串提取、查找和区间修改。

**典型场景：读取并处理文本、解析输入、识别子串、按规则构造或修改字符串。**

```cpp
string s = "abcdef";
cout << s[2];           // c
cout << s.substr(2, 3); // cde
```

注意 `substr(pos, count)` 的第二个参数是**字符个数**，不是结束下标。

---

## 2. 常用 API

以下以 `string`（即 `basic_string<char>`）为例。表中写出常用重载的参数、返回类型；省略少用重载及不影响使用的部分限定符。

| API 签名 | 作用与返回值 | 时间复杂度 |
|---|---|---|
| `size_type size() const` | 返回字符数 | \(O(1)\) |
| `bool empty() const` | 判断是否为空 | \(O(1)\) |
| `char& operator[](size_type pos)` | 按下标访问；不进行越界检查 | \(O(1)\) |
| `char& front()` / `char& back()` | 访问首 / 尾字符 | \(O(1)\) |
| `void push_back(char ch)` | 追加一个字符 | 均摊 \(O(1)\) |
| `void pop_back()` | 删除末尾字符，不返回值 | \(O(1)\) |
| `string& operator+=(char ch)` | 末尾追加一个字符，返回自身引用 | 均摊 \(O(1)\) |
| `string& operator+=(const string& str)` | 末尾拼接字符串，返回自身引用 | 均摊 \(O(m)\) |
| `string substr(size_type pos = 0, size_type count = npos) const` | 返回复制出的子串 | \(O(\text{实际子串长度})\) |
| `size_type find(const string& str, size_type pos = 0) const` | 从 `pos` 起找子串，返回首位置或 `npos` | 取决于实现与匹配情况 |
| `size_type find(char ch, size_type pos = 0) const` | 从 `pos` 起找字符，返回位置或 `npos` | 最坏 \(O(n)\) |
| `string& erase(size_type pos = 0, size_type count = npos)` | 删除指定区间，返回自身引用 | 最坏 \(O(n)\) |
| `string& insert(size_type pos, const string& str)` | 在 `pos` 前插入字符串，返回自身引用 | 最坏 \(O(n+m)\) |
| `string& replace(size_type pos, size_type count, const string& str)` | 替换指定区间，返回自身引用 | 最坏 \(O(n+m)\) |
| `int compare(const string& str) const` | 与整个字符串按字典序比较 | \(O(\min(n,m))\) |
| `int compare(size_type pos, size_type count, const string& str) const` | 将指定子串与 `str` 比较 | \(O(\min(count,m))\) |
| `void clear()` | 清空字符串 | \(O(n)\) |

其中 `n` 为当前字符串长度，`m = str.size()`；复杂度按通常的字符处理成本讨论。

### 查找、截取、修改

```cpp
string s = "abcabc";

auto p = s.find("bc");      // 1
auto q = s.find("bc", 2);   // 4

string t = s.substr(1, 3); // "bca"
s.erase(2, 2);             // "abbc"
s.insert(2, "XY");         // "abXYbc"
s.replace(2, 2, "12");     // "ab12bc"

string stars(3, '*');     // "***"，重复字符构造
```

若要提取闭区间 `[l, r]`：

```cpp
string t = s.substr(l, r - l + 1);
```

若要查找而非修改：

```cpp
auto pos = s.find("abc");
if (pos != string::npos) {
    cout << pos << '\n';
}
```

`string::npos` 表示未找到；查找位置的类型通常是无符号的 `string::size_type`，建议用 `auto` 或 `size_t` 保存，**不要习惯性与 `-1` 比较**。

### `compare()`：无需构造临时子串

```cpp
string s = "hello world";
string w = "world";

if (s.compare(6, 5, w) == 0) {
    // s 中 [6, 11) 与 w 相同
}
```

```cpp
s.substr(6, 5) == w;       // 先创建一个新 string 再比较
s.compare(6, 5, w) == 0;   // 直接比较指定区间
```

- 返回 `< 0`：字典序更小；`== 0`：相等；`> 0`：更大。
- 返回值**不保证恰好是** `-1`、`0`、`1`。
- 比较两个**完整**字符串时，直接使用 `a == b` 更简洁。
- `compare()` 节省临时子串分配，但不意味着渐近复杂度必然更优。

### 常用非成员函数与数值转换

| API 签名 | 用途 |
|---|---|
| `istream& getline(istream& is, string& str)` | 读取整行（允许空格），不保留行尾换行符 |
| `int stoi(const string& str, size_t* pos = nullptr, int base = 10)` | 字符串转 `int` |
| `long long stoll(const string& str, size_t* pos = nullptr, int base = 10)` | 字符串转 `long long` |
| `string to_string(int value)` | 整数转字符串（另有其他数值类型重载） |

```cpp
int x = stoi("123");
long long y = stoll("1234567890123");
string s = to_string(2026);
```

`stoi()`、`stoll()` 在无法转换或结果越界时会抛出异常；解析竞赛题时通常依赖题目保证的输入格式。

---

## 3. 高频坑点

### 3.1 `cin >> s` 与 `getline(cin, s)`

```cpp
cin >> s;        // 遇空白停止
getline(cin, s); // 读入整行，可包含空格
```

如果前面通过 `cin >> w` 读取了单词，随后用 `getline` 读取文章，需要先丢弃上一行剩余内容：

```cpp
string w, text;
cin >> w;
cin.ignore(numeric_limits<streamsize>::max(), '\n');
getline(cin, text);
```

在明确知道只剩一个 `\n` 的情况下，`cin.ignore()` 也可行。

### 3.2 `substr()` 的长度和位置

```cpp
string s = "abcdef";
s.substr(2, 3); // "cde"，不是 [2, 3]
```

- `pos > s.size()`：抛出 `out_of_range`。
- `count` 超过剩余长度：截取到字符串末尾，不会越界读取。

### 3.3 `find()` 返回 `npos`，不是一个有效下标

```cpp
auto p = s.find("xyz");
if (p != string::npos) {
    cout << s[p];
}
```

### 3.4 边遍历边 `erase()` 容易跳过元素

```cpp
string s = "a--bc";
for (int i = 0; i < (int)s.size(); ++i) {
    if (s[i] == '-') s.erase(i, 1);
}
```

删除后，后方字符左移，下一轮 `++i` 可能跳过补到当前位置的字符。若无原地修改要求，可直接构造答案字符串：

```cpp
string ans;
for (char c : s) {
    if (c != '-') ans.push_back(c);
}
```

### 3.5 大小写转换的字符类型

```cpp
for (char& c : s) {
    c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
}
```

`tolower()` 接受 `EOF` 或能表示为 `unsigned char` 的值。对于只含 ASCII 英文字母的竞赛题，也可以自行判断 `'A' <= c && c <= 'Z'` 后转换。

---

## 4. 代表题：[P1308 [NOIP2011 普及组] 统计单词数](https://www.luogu.com.cn/problem/P1308)

**题型：字符串查找、单词边界处理、大小写归一化。**

给定目标单词和一篇文章，求目标单词作为**完整单词**出现的次数及第一次出现的位置（从 0 开始），匹配不区分大小写。

关键困难不是找到目标子串，而是避免将 `to` 错误匹配到 `together` 中。

### 4.1 独立解法：先分词，再比较

原思路：用 `find(' ', l)` 找单词右边界，`substr(l, r-l)` 提取一个完整单词，再与目标比较。

```cpp
size_t l = 0;
int cnt = 0, first = -1;

while (l < text.size()) {
    size_t r = text.find(' ', l);
    if (r == string::npos) r = text.size();

    string part = text.substr(l, r - l);
    if (part == word) {
        if (cnt == 0) first = static_cast<int>(l);
        ++cnt;
    }

    l = r + 1;
}
```

**评价：正确、天然保证完整单词匹配，整体线性。** 由于单词片段互不重叠，`find(' ')` 的扫描与 `substr()` 的总复制量均为 \(O(n)\)；每个片段的字符串比较也不会超过片段自身长度。

原版中将 `find()` 结果赋给 `ll`、以 `len + 1` 表示未找到，没有必要；改为 `size_t`，并统一使用 `text.size()` 作为最终右边界即可。

只想省掉临时子串时，可把判断替换为：

```cpp
if (text.compare(l, r - l, word) == 0) {
    if (cnt == 0) first = static_cast<int>(l);
    ++cnt;
}
```

**这属于实现层面优化，不改变整体 \(O(n)\) 复杂度。**

### 4.2 另一种建模：两端补空格 + `find()`

这与原解是真正不同的处理方式，因此单独展示。

将目标单词与文章两端补空格：

```text
目标： "to"       → " to "
文章： "to be or not to be"
补后： " to be or not to be "
```

于是**单词边界检查转化为普通子串匹配**：只有被空格包围的完整单词才可能匹配。文章左侧额外插入一个空格，恰好使找到的子串起始下标等于原文中该单词的起始位置。

**推荐的完整实现（C++11）：**

```cpp
#include <bits/stdc++.h>
using namespace std;

void solve() {
    string word, text;
    cin >> word;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, text);

    for (char& c : word)
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    for (char& c : text)
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));

    word = " " + word + " ";
    text = " " + text + " ";

    int cnt = 0, first = -1;
    size_t pos = text.find(word);

    while (pos != string::npos) {
        if (cnt == 0) first = static_cast<int>(pos);
        ++cnt;
        pos = text.find(word, pos + 1);
    }

    if (cnt == 0) cout << -1 << '\n';
    else cout << cnt << ' ' << first << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
}
```

**比较：**

| 思路 | 优势 | 注意点 |
|---|---|---|
| 分词后比较 | 时间复杂度可明确保证为 \(O(n)\)，思路直观 | 需要处理每个单词的边界 |
| 两端补空格再查找 | 建模简洁，消除逐次判断边界的分支 | `string::find()` 的最坏时间复杂度不保证线性；需要题目以空格分隔单词 |

这里的优秀之处主要是**改变问题表示，而不是提升渐近复杂度**。

### 4.3 思维提炼

> **如果答案需要满足边界条件，可以尝试添加哨兵字符，让边界条件成为普通匹配规则的一部分。**

与扫雷题在外围添加 padding 的思路相通：与其不断特判边界，不如先改变数据表示。

---

## 5. 选型触发器

- **需要提取固定区间** → `substr(pos, count)`；只需比较区间时，可考虑 `compare(pos, count, str)`。
- **查找字符 / 子串** → `find()`，检查 `npos`。
- **整行文本含空格** → `getline()`，注意与 `cin >>` 混用。
- **频繁修改中间位置** → 注意移动成本和遍历下标失效；必要时构造新字符串。
- **完整单词匹配** → 按空格分词，或在题目允许时通过补空格转化匹配条件。

**一句话：`string` 常用操作要分清“下标位置”和“子串长度”，更要学会通过调整输入表示简化边界规则。**
