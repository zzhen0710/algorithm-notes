#pragma once
#include <algorithm>
#include <functional>
#include <vector>

// 同容量一维装箱：全部正重量对象都要装入，返回最少箱数。
// cap>0；cap、单件重量<=1e18；建议 n<=18；空集返回0，单件超容量返回-1。
// 按值接收并排序，不改调用处数组；只求数量，不返回分组。
inline int min_bins(std::vector<long long> w, long long cap) {
    int n = (int)w.size(), best = n;
    for (long long x : w)
        if (x > cap) return -1;
    std::sort(w.rbegin(), w.rend());
    std::vector<long long> load(n);

    std::function<void(int, int)> dfs = [&](int i, int used) {
        if (used >= best) return;
        if (i == n) {
            best = used;
            return;
        }
        for (int j = 0; j < used; ++j) {
            if (w[i] > cap - load[j]) continue;
            load[j] += w[i];
            dfs(i + 1, used);
            load[j] -= w[i];
        }
        load[used] = w[i]; // 新箱统一用 used 编号；父层不读这个槽位。
        dfs(i + 1, used + 1);
    };
    dfs(0, 0);
    return best;
}
