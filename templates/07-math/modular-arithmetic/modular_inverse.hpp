#pragma once

#include "../binary-exponentiation/binary_exponentiation.hpp"

// p 为素数，2<=p<2^31；a 可为任意 ll。调用者保证素数，不在此检测。
// 返回 0..p-1 中的逆元；a%p=0 时返回 -1。
inline ll inv_prime(ll a, ll p) {
    a %= p;
    if (a < 0) a += p;
    return a == 0 ? -1 : mod_pow(a, p - 2, p);
}
