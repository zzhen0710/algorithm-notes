# 非负高精度整数：十进制数位

[知识笔记与推导](../../../notes/07-math/08-big-integer.md) · [实现](big_integer.hpp)

## 接口与契约

使用 C++11；全部接口位于 `bigint` 命名空间。只处理非负整数，不含负数、高精度除以高精度或小数。

- **表示：** `Big = std::vector<int>`，0-based；a[i] 是 10ⁱ 位数字，范围 0..9；非空且无多余高位零，整数 0 用 `{0}`。
- **输入：** read_big(s) 要求非空十进制数字串，允许前导零；不接受负号、加号、小数点、空串或其他字符，不做非法输入检查。
- **长度：** 各输入位数不超过 INT_MAX/2，所有结果有足够内存；加法最多多一位，乘法最多 n+m 位。不按数值大小开数组。
- **中间量：** 十进制加法局部值≤19，乘法局部值≤99；div_small 使用 ll 存余数与余数乘 10。按常见 32 位 int／64 位 ll 环境设计。
- **副作用与多测：** 除 trim 原地删高位零外，其余接口不修改输入，返回独立结果；没有共享状态，每组重新构造 Big。
- **统一表示：** read_big 与运算结果均满足契约；手工修改数位后也须保持约定。trim 不校验各位范围，也不把空容器自动改成 `{0}`。

| 接口 | 返回内容 | 时间 | 额外空间（含结果） |
| --- | --- | --- | --- |
| read_big(s) | 数字串对应的 Big | O(n) | O(n) |
| str_big(a) | 无前导零的十进制字符串 | O(n) | O(n) |
| cmp(a,b) | −1／0／1，表示小于／等于／大于 | 最坏 O(max(n,m)) | O(1) |
| add(a,b) | A+B | O(max(n,m)) | O(max(n,m)) |
| sub(a,b) | A−B，调用者保证 A≥B | O(n) | O(n) |
| mul(a,b) | A×B | O(nm) | O(n+m) |
| div_small(a,b) | `{商,余数}`，要求 1≤b≤INT_MAX | O(n) | O(n) |
| trim(a) | 原地规范高位零，返回 void | 最坏 O(n) | O(1) |

n、m 是有效数位数；read_big 的 n 是输入串长度。单独取出 div_small 的商和余数后，有 A=b×Q+r、`0 ≤ r < b`。

## 使用示例

```cpp
#include "big_integer.hpp"
#include <iostream>

int main() {
    using namespace bigint;
    Big a = read_big("0012030"), b = read_big("70");

    std::cout << str_big(add(a, b)) << '\n'; // 12100
    std::cout << str_big(sub(a, b)) << '\n'; // 11960；已知 a >= b
    std::cout << str_big(mul(a, b)) << '\n'; // 842100

    auto result = div_small(read_big("1234"), 12);
    std::cout << str_big(result.first) << ' ' << result.second << '\n'; // 102 10
    std::cout << cmp(read_big("19"), read_big("20")) << '\n'; // -1
}
```

竞赛模板按已确认风格使用 `(int)` 等简短数值转换，压缩部分分支和注释，保留相同数位约定；加命名空间避免通用 add、sub 等名称与其他模板冲突。复制到单文件时可整体保留命名空间，或明确移除后再调用。

表示与竖式原理参考链接保留在知识笔记；实现独立整理，不移植大进制版本。通用运算与两题实际实现已本地核验，复盘见[高精度典题](../../../notes/07-math/08-big-integer.md#7-典题与复盘)；P1601 原稿的前导零补强独立列出，P1255 的 TLE／RE 反馈与当前实现分别记录。既有 add 即可复用，不新增斐波那契专属模板。

[本章模板](../README.md) · [全部模板](../../README.md)
