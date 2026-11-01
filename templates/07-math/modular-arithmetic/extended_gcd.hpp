#pragma once

using ll = long long;

// 0<=a,b<=2e9；返回 gcd，并写入 a*x+b*y=gcd 的一组整数系数。
// x、y 必须是两个不同的输出变量；约定 gcd(0,0)=0。
inline ll exgcd(ll a, ll b, ll& x, ll& y) {
    if (!b) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll g = exgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - a / b * y1;
    return g;
}

// a 可为任意 ll；2<=mod<=2e9。返回 [0,mod) 中的逆元，无解返回 -1。
inline ll inv_general(ll a, ll mod) {
    a %= mod;
    if (a < 0) a += mod;
    ll x, y;
    if (exgcd(a, mod, x, y) != 1) return -1;
    x %= mod;
    return x < 0 ? x + mod : x;
}
