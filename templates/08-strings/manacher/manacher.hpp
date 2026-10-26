#pragma once

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

// 0-based；n <= INT_MAX/4，内存足够。按字节比较，不修改 s。
// t 长 2n+1，偶数位置为 -1；p[i] 为不含中心的半径，也是原串回文长度。
inline std::vector<int> manacher(const std::string& s) {
    int n = (int)s.size(), m = 2 * n + 1;
    std::vector<int> t(m, -1), p(m);
    for (int i = 0; i < n; ++i) t[2 * i + 1] = (unsigned char)s[i];

    int center = 0, right = -1;
    for (int i = 0; i < m; ++i) {
        if (i <= right) p[i] = std::min(p[2 * center - i], right - i);
        while (i - p[i] - 1 >= 0 && i + p[i] + 1 < m &&
               t[i - p[i] - 1] == t[i + p[i] + 1]) ++p[i];
        if (i + p[i] > right) center = i, right = i + p[i];
    }
    return p;
}

// p 由 manacher 生成；返回 {原串起点, 长度}，同长取最左，空串 {0,0}。
inline std::pair<int, int> longest_palindrome(const std::vector<int>& p) {
    int start = 0, len = 0;
    for (int i = 0; i < (int)p.size(); ++i)
        if (p[i] > len) len = p[i], start = (i - p[i]) / 2;
    return {start, len};
}

// p 对应同一原串；0 <= l <= r <= n，原串左闭右开区间，空区间为回文。
inline bool is_palindrome(const std::vector<int>& p, int l, int r) {
    return p[l + r] >= r - l;
}
