#pragma once

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

namespace bigint {

using ll = long long;
using Big = std::vector<int>;

// 十进制，低位在前；唯一零表示 {0}，不接受空容器。
inline void trim(Big& a) {
    while (a.size() > 1 && a.back() == 0) a.pop_back();
}

// s 非空且只有数字，允许前导零。
inline Big read_big(const std::string& s) {
    Big a;
    for (int i = (int)s.size() - 1; i >= 0; --i)
        a.push_back(s[i] - '0');
    trim(a);
    return a;
}

inline std::string str_big(const Big& a) {
    std::string s;
    for (int i = (int)a.size() - 1; i >= 0; --i)
        s.push_back('0' + a[i]);
    return s;
}

inline int cmp(const Big& a, const Big& b) {
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    for (int i = (int)a.size() - 1; i >= 0; --i)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}

inline Big add(const Big& a, const Big& b) {
    Big c;
    int carry = 0, n = (int)std::max(a.size(), b.size());
    for (int i = 0; i < n; ++i) {
        int cur = carry;
        if (i < (int)a.size()) cur += a[i];
        if (i < (int)b.size()) cur += b[i];
        c.push_back(cur % 10);
        carry = cur / 10;
    }
    if (carry) c.push_back(carry);
    return c;
}

// 要求 a >= b。
inline Big sub(const Big& a, const Big& b) {
    Big c(a.size());
    int borrow = 0;
    for (int i = 0; i < (int)a.size(); ++i) {
        int cur = a[i] - borrow;
        if (i < (int)b.size()) cur -= b[i];
        borrow = (cur < 0);
        c[i] = cur + (borrow ? 10 : 0);
    }
    trim(c);
    return c;
}

inline Big mul(const Big& a, const Big& b) {
    int n = (int)a.size(), m = (int)b.size();
    Big c(n + m);
    for (int i = 0; i < n; ++i) {
        int carry = 0;
        for (int j = 0; j < m; ++j) {
            int cur = c[i + j] + a[i] * b[j] + carry;
            c[i + j] = cur % 10;
            carry = cur / 10;
        }
        c[i + m] = carry; // 本行末尾是此前未写入的更高位
    }
    trim(c);
    return c;
}

// b > 0 且可用 int 表示；返回 {整数商, 余数}。
inline std::pair<Big, int> div_small(const Big& a, int b) {
    Big q(a.size());
    ll rem = 0;
    for (int i = (int)a.size() - 1; i >= 0; --i) {
        ll cur = rem * 10 + a[i];
        q[i] = (int)(cur / b);
        rem = cur % b;
    }
    trim(q);
    return {q, (int)rem};
}

} // namespace bigint
