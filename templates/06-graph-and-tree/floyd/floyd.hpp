#pragma once

#include <algorithm>
#include <limits>
#include <vector>

using ll = long long;
constexpr ll FLOYD_INF = std::numeric_limits<ll>::max() / 4;

// 1-based 方阵；对角线 0、无边 INF，重边先取 min；无负环。
// 有限状态及候选和都须在 (-FLOYD_INF,FLOYD_INF) 内；原地求全源距离。
inline void floyd(std::vector<std::vector<ll>>& d) {
    int n = static_cast<int>(d.size()) - 1;
    for (int k = 1; k <= n; ++k)
        for (int i = 1; i <= n; ++i) {
            if (d[i][k] == FLOYD_INF) continue;
            for (int j = 1; j <= n; ++j) {
                if (d[k][j] == FLOYD_INF) continue;
                d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);
            }
        }
}
