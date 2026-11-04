#pragma once

#include <vector>

using ll = long long;

// n>=0，moves 的每项为正；不能操作者输。返回下标0..n的当前玩家胜负表。
// 不修改输入，空moves全部为败；O(n*(|moves|+1))时间、O(n)空间。
inline std::vector<char> subtraction_wins(int n, const std::vector<int>& moves) {
    std::vector<char> win(n + 1, false); // win[0]=false：没有石子可取，当前玩家输。

    for (int x = 1; x <= n; ++x) {
        for (int take : moves) {
            if (take > x) continue;

            // 走到 x-take 后轮到对手；能让对手必败，自己就必胜。
            if (!win[x - take]) {
                win[x] = true;
                break;
            }
        }
    }
    return win;
}

// 标准Nim：每次只改一堆，任意取正数颗，不能操作者输；堆大小非负。
// 空数组/全0为false；O(堆数)时间、O(1)额外空间，不修改输入。
inline bool nim_first_wins(const std::vector<ll>& piles) {
    ll s = 0;
    for (ll x : piles)
        s ^= x;
    return s != 0;
}
