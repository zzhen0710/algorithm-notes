#pragma once

using ll = long long;

// 整数闭区间，0 <= L <= R <= 10^18
// check：false -> true；check(R) 为真，范围内至少有一个可行值
// 每次 check 只依赖本次候选，对同一候选返回一致结果
template<class Predicate>
inline ll firstTrue(ll L, ll R, Predicate check) {
    while (L < R) {
        ll mid = L + (R - L) / 2;

        if (check(mid)) {
            R = mid; // 可行，保留它并尝试更小值
        } else {
            L = mid + 1; // 排除所有不超过 mid 的候选
        }
    }

    return L;
}

// 同样的范围约定；check：true -> false，check(L) 为真
template<class Predicate>
inline ll lastTrue(ll L, ll R, Predicate check) {
    while (L < R) {
        // 向上取中点，与可行时保留 L=mid 配套
        ll mid = L + (R - L + 1) / 2;

        if (check(mid)) {
            L = mid;
        } else {
            R = mid - 1;
        }
    }

    return L;
}
