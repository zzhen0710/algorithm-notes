#pragma once
#include <vector>

using Mat = std::vector<std::vector<long long>>;

// A 为 n×r，B 为 r×m；矩阵非空且行长一致，元素已在 [0,mod) 内。
// 1<=mod<2^31，逐项取模保证 long long 乘加安全。
inline Mat mat_mul(const Mat& a, const Mat& b, long long mod) {
    int n = (int)a.size(), r = (int)b.size(), m = (int)b[0].size();
    Mat c(n, std::vector<long long>(m));
    for (int i = 0; i < n; ++i)
        for (int t = 0; t < r; ++t) {
            if (a[i][t] == 0) continue;
            for (int j = 0; j < m; ++j)
                c[i][j] = (c[i][j] + a[i][t] * b[t][j]) % mod;
        }
    return c;
}

// a 为非空方阵，k>=0；输入元素可为任意 long long，先规范余数。
// 返回 A^k mod mod，k=0 返回单位矩阵；按值接收，不改调用处 a。
inline Mat mat_pow(Mat a, long long k, long long mod) {
    int n = (int)a.size();
    for (auto& row : a)
        for (auto& x : row) {
            x %= mod;
            if (x < 0) x += mod;
        }
    Mat ans(n, std::vector<long long>(n));
    for (int i = 0; i < n; ++i) ans[i][i] = 1 % mod;
    while (k > 0) {
        if (k & 1) ans = mat_mul(ans, a, mod);
        a = mat_mul(a, a, mod);
        k >>= 1;
    }
    return ans;
}
