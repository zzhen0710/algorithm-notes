#pragma once

#include <vector>

using ll = long long;

// a、b 非负，约定 gcd(0,0)=0；无外部副作用
inline ll igcd(ll a, ll b) {
    return b == 0 ? a : igcd(b, a % b);
}

// a、b 非负，最终 lcm 必须能用 ll 表示；任一为 0 时返回 0
inline ll ilcm(ll a, ll b) {
    if (a == 0 || b == 0) return 0;
    return a / igcd(a, b) * b; // 先除再乘，但不检测最终答案溢出
}

// 元素非负，不修改 a；空数组或全零数组返回 0
inline ll agcd(const std::vector<ll>& a) {
    ll g = 0;
    for (ll x : a) g = igcd(g, x);
    return g;
}
