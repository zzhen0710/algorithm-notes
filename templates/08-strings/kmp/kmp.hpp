#pragma once

#include <string>
#include <vector>

// pi[i]：s[0..i] 的最长相等真前后缀长度；空串返回空数组
// 前提：字符串长度可用 int 表示；不修改输入
inline std::vector<int> prefix_function(const std::string& s) {
    int n = static_cast<int>(s.size());
    std::vector<int> pi(n);

    for (int i = 1, j = 0; i < n; ++i) {
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) ++j;
        pi[i] = j;
    }

    return pi;
}

// 返回 text 中 pattern 的全部 0-based 起点，按递增顺序，允许重叠
// 本接口约定空模式返回空结果；两个字符串的长度均可用 int 表示
inline std::vector<int> kmp_find_all(const std::string& text,
                                     const std::string& pattern) {
    std::vector<int> pos;
    if (pattern.empty()) return pos;

    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    std::vector<int> pi = prefix_function(pattern);

    for (int i = 0, j = 0; i < n; ++i) {
        while (j > 0 && text[i] != pattern[j]) j = pi[j - 1];
        if (text[i] == pattern[j]) ++j;

        if (j == m) {
            pos.push_back(i - m + 1);
            j = pi[j - 1]; // 完整匹配后保留 border，下一次仍可重叠
        }
    }

    return pos;
}
