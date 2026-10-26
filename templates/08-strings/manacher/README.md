# Manacher：半径、最长回文与区间判断

[知识笔记与连续执行图](../../../notes/08-strings/03-manacher.md#2-跟着代码跑完一个例子) · [实现](manacher.hpp)

## 接口与约定

- **manacher(s)：** 输入固定字符串，返回长度 2n+1 的 p。改造序列偶数下标为分隔符 −1，原串第 q 个字节在 2q+1；按 unsigned char 转成 0..255，不与分隔符冲突。
- **半径：** p[i] 不含中心，改造序列回文范围为 [i−p[i],i+p[i]]；在本约定下，它同时等于对应原串回文的长度。
- **longest_palindrome(p)：** 返回 `{原串起点,长度}`，同长取最左。调用时传入已经算出的 p，不重复预处理原串。
- **is_palindrome(p,l,r)：** 查询原串区间 [l,r) 是否回文；要求 0≤l≤r≤n，p 必须属于这一原串。
- **空串：** manacher 返回 `{0}`，最长返回 `{0,0}`；空区间判断为 true。p 本身不允许用空 vector 代替接口结果。
- **范围与内存：** n≤INT_MAX/4，保证长度、镜像与下标表达式用 int 计算安全；调用者还要保证内存足够。构造时 t、p 共约 4n 个 int，另有原字符串。长度是字节数。
- **副作用与多测：** 输入只读，结果为独立局部数组；无共享状态，每次调用重新初始化。此表针对固定原串，修改字符后须重算，不能继续用旧 p 查询。
- **依赖：** 标准头文件、C++11；普通数值转换采用 `(int)` 等竞赛短写法，无外部库。

## 复杂度

| 操作 | 时间 | 空间 |
| --- | --- | --- |
| manacher | O(n) | O(n)，含返回半径表及临时改造数组 |
| longest_palindrome | O(n) | 额外 O(1) |
| is_palindrome | O(1) | 额外 O(1) |
| s.substr(start,len) | O(len) | 新字符串 O(len) |

半径表紧凑表示回文信息；显式枚举全部回文子串仍受输出数量限制。

## 使用示例

```cpp
#include "manacher.hpp"
#include <iostream>

int main() {
    std::string s = "ababc";
    auto p = manacher(s);
    auto best = longest_palindrome(p);
    std::cout << best.first << ' ' << best.second << '\n'; // 0 3
    std::cout << s.substr(best.first, best.second) << '\n'; // aba
    std::cout << is_palindrome(p, 1, 4) << '\n'; // 1，bab
    std::cout << is_palindrome(p, 0, 4) << '\n'; // 0，abab
    std::cout << is_palindrome(p, 2, 2) << '\n'; // 1，空区间
}
```

## 复现重点

1. 半径不含中心、right 为闭区间右端；初始 right=−1。
2. 镜像只复用到当前 right，用 min 限制，再从半径外尝试扩展。
3. 更新条件是 i+p[i]>right，不能只比较 p[i] 谁更大。
4. 查询原串 [l,r) 用中心 l+r 与所需长度 r−l，不把原串下标直接当作改造序列下标。

模板为教学版本的精简实现，沿用同一半径定义与字节契约；算法参考保留在笔记。通用实现与示例已本地核验；[P3805 与 CF1326D2 两题复盘](../../../notes/08-strings/03-manacher.md#7-典题与复盘)已整理并本地核验，平台结果未单独提供。两题复用本组 manacher，不将题目专属选择策略混入通用接口。

[本章模板](../README.md) · [全部模板](../../README.md)
