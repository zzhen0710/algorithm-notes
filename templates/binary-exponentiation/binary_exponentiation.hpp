#pragma once

using ll = long long;

// a 可为任意 ll；0<=b<=LLONG_MAX，1<=p<2^31，保证 ll 模乘安全
// 返回 a^b 的规范余数；零次幂按 1 处理，无外部副作用
inline ll mod_pow(ll a, ll b, ll p) {
    a %= p;
    if (a < 0) a += p; // 先规范到底数余数范围 0..p-1

    ll ans = 1 % p; // p=1 时，零次幂也返回 0

    while (b > 0) {
        if (b & 1)
            ans = ans * a % p; // 选入当前位对应的幂

        a = a * a % p; // 平方得到下一位对应的幂
        b >>= 1;       // 移除已经处理的最低位
    }

    return ans;
}
