# 字符串双哈希：子串指纹与去重键

使用 C++11。实现：[string_hash.hpp](string_hash.hpp)。

从 [8.2 的通用实现与两题复盘](../../notes/08-strings/02-string-hash.md)提炼，保留固定参数与四个前缀/幂数组；增加 `key(l,r)`，减少去重时手动拼接长度的重复代码。

## 接口与约定

- `StringHash(s)`：预处理字符串的**字节序列**，编码为 `unsigned char + 1`，范围 1..256，区分大小写。

- `get(l,r)`：返回 `pair<ll,ll>`，是 **0-based 半开区间 [l,r)** 的两份哈希。

- `key(l,r)`：返回 `StringHash::Key`，即 `tuple<int,ll,ll>`，依次保存**长度、第一份哈希、第二份哈希**。用于指纹比较或排序去重时优先使用它。

- 前提：字符串长度及 n+1 可用 int 表示，内存足够；查询满足 `0 <= l <= r <= n`。`n` 为原长度，只读取，不改写表及固定参数。

- 空串可构造；空区间 `get(l,l)` 为 `{0,0}`，`key(l,l)` 为 `{0,0,0}`。两个对象使用同一套参数，可以跨字符串比较。

- 构造不修改输入，**只保存前缀表，不保存原文**；原串之后变化不会同步到表，需要重建。各对象独立初始化，多测无需清理全局状态。

**键相同仅表示长度和两份指纹相同，仍可能碰撞。** 严格相等需要原文复核；不提供名字容易误导的精确相等接口。

## 区间换算

| 题目区间 | 调用 |
| --- | --- |
| 0-based [l,r) | get(l,r) 或 key(l,r) |
| 0-based [l,r] | get(l,r+1) 或 key(l,r+1) |
| 1-based [L,R] | get(L-1,R) 或 key(L-1,R) |
| 1-based [L,R) | get(L-1,R-1) 或 key(L-1,R-1) |

原因与边界图解见 [8.2 的传参说明](../../notes/08-strings/02-string-hash.md#区间为什么这样传参)。

## 使用示例

```cpp
#include "string_hash.hpp"
#include <algorithm>
#include <iostream>

int main() {
    StringHash a("ababa"), b("aba");

    // 三段内容均为 aba，位置与所属原串可以不同
    std::cout << (a.key(0, 3) == a.key(2, 5)) << '\n'; // 1
    std::cout << (a.key(0, 3) == b.key(0, 3)) << '\n'; // 1

    std::vector<StringHash::Key> keys = {
        a.key(0, 3), a.key(2, 5), a.key(1, 3)
    };
    std::sort(keys.begin(), keys.end());

    std::cout << std::unique(keys.begin(), keys.end()) - keys.begin()
              << '\n'; // 2：aba、ba
}
```

头文件自带所需标准头，不依赖 `using namespace std;`。不包含 main，不负责输入输出或候选合法性判断。

## 成本与数值边界

- 构造时间 O(n)，四个数组共 O(n) 空间。
- `get/key` 单次时间和返回值空间均为 O(1)。
- 后续排序 K 个键另计 O(K log(K+1)) 时间和键容器空间。
- 当前约十亿的模数保证中间乘积在 ll 内；不是先发生 int 乘法再扩大结果。更换参数时重新检查乘积。
- 固定 BASE 与两个模数**不提供绝对无碰撞或固定概率的承诺**，说明见[碰撞讨论](../../notes/08-strings/02-string-hash.md#6-为什么双哈希也不是严格证明)。

模板已完成本地数值、区间换算、跨对象比较、空串与字节边界核验；示例和两道复盘代码均已本地对照验证。平台结果、独立复现与熟练度分别记录。

配套思维模型见[筛选候选与比较键](../../patterns/08-strings/filter-key-dedup.md)。
