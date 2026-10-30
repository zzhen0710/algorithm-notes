#pragma once
#include <algorithm>
#include <cmath>
#include <vector>

struct GaussRes {
    int status; // 0 无解，1 唯一解，2 无穷多解（按 eps 判定）。
    int rank;   // 系数部分找到的主元数。
    std::vector<double> x; // 有解时返回一组解，自由变量取 0；无解为空。
};

// 实数方程组的增广矩阵：n 行、m+1 列，0 基，最后一列为常数。
// n,m>=1，各行等长、元素有限、eps>0；按值传入，不改调用处矩阵。
inline GaussRes gauss(std::vector<std::vector<double>> a, double eps = 1e-9) {
    int n = (int)a.size(), m = (int)a[0].size() - 1;
    int row = 0;
    std::vector<int> where(m, -1);

    for (int col = 0; col < m && row < n; ++col) {
        int sel = row;
        for (int i = row + 1; i < n; ++i)
            if (std::fabs(a[i][col]) > std::fabs(a[sel][col])) sel = i;
        if (std::fabs(a[sel][col]) <= eps) continue;

        std::swap(a[sel], a[row]);
        where[col] = row; // 主元已换到 row；sel 是交换前的位置。
        double pivot = a[row][col];
        for (int j = col; j <= m; ++j) a[row][j] /= pivot;

        for (int i = row + 1; i < n; ++i) {
            double factor = a[i][col];
            a[i][col] = 0;
            for (int j = col + 1; j <= m; ++j)
                a[i][j] -= factor * a[row][j];
        }
        ++row;
    }

    for (int i = row; i < n; ++i)
        if (std::fabs(a[i][m]) > eps) return {0, row, {}};

    std::vector<double> x(m, 0);
    for (int col = m - 1; col >= 0; --col) {
        int r = where[col];
        if (r == -1) continue;
        x[col] = a[r][m];
        for (int j = col + 1; j < m; ++j) x[col] -= a[r][j] * x[j];
    }
    return {row == m ? 1 : 2, row, x};
}
