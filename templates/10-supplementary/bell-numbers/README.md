# Bell 数取模预处理

`bell_numbers(n, mod)` 要求 `n >= 0` 且 `mod` 为正 `int`，返回 `B[0..n]` 的模数余数。一次预处理 $O(n^2)$，空间 $O(n)$，随后单项查询 $O(1)$。组合数由一维杨辉三角递推，模数可以为合数。

[实现](bell_numbers.hpp) · [证明与使用时机](../../../notes/10-supplementary/01-bell-numbers.md) · [本章索引](../README.md)
