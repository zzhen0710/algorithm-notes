#pragma once

#include <vector>

using ll = long long;

// a[1..n]，闭区间；x->c*x+d与区间和取模。
// 1<=n<=2e5，1<=mod<=1e9+9；公有更新接口会规范化任意ll系数。
struct LazyAffineSum {
    int n;
    ll mod;
    std::vector<ll> tr, tm, ta;

    LazyAffineSum(const std::vector<ll>& a, ll mod)
        : n(static_cast<int>(a.size()) - 1), mod(mod),
          tr(4 * a.size()), tm(4 * a.size(), 1 % mod), ta(4 * a.size()) {
        build(1, 1, n, a);
    }

    ll norm(ll x) const { x %= mod; return x < 0 ? x + mod : x; }

    void transform(int l, int r, ll c, ll d) {
        update(1, 1, n, l, r, norm(c), norm(d));
    }
    void add(int l, int r, ll d) { transform(l, r, 1, d); }
    void multiply(int l, int r, ll c) { transform(l, r, c, 0); }
    ll query(int l, int r) { return query(1, 1, n, l, r); }

    void pull(int p) { tr[p] = (tr[p * 2] + tr[p * 2 + 1]) % mod; }

    void build(int p, int l, int r, const std::vector<ll>& a) {
        if (l == r) { tr[p] = norm(a[l]); return; }
        int mid = l + (r - l) / 2;
        build(p * 2, l, mid, a);
        build(p * 2 + 1, mid + 1, r, a);
        pull(p);
    }

    void apply(int p, int l, int r, ll c, ll d) {
        tr[p] = (c * tr[p] + d * (r - l + 1)) % mod;
        tm[p] = c * tm[p] % mod;       // 新操作接在旧标记之后。
        ta[p] = (c * ta[p] + d) % mod;
    }

    void push(int p, int l, int r) {
        if (l == r || (tm[p] == 1 % mod && ta[p] == 0)) return;
        int mid = l + (r - l) / 2;
        apply(p * 2, l, mid, tm[p], ta[p]);
        apply(p * 2 + 1, mid + 1, r, tm[p], ta[p]);
        tm[p] = 1 % mod;
        ta[p] = 0;
    }

    void update(int p, int l, int r, int ql, int qr, ll c, ll d) {
        if (ql <= l && r <= qr) { apply(p, l, r, c, d); return; }
        push(p, l, r);
        int mid = l + (r - l) / 2;
        if (ql <= mid) update(p * 2, l, mid, ql, qr, c, d);
        if (qr > mid) update(p * 2 + 1, mid + 1, r, ql, qr, c, d);
        pull(p);
    }

    ll query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p];
        push(p, l, r);
        int mid = l + (r - l) / 2;
        ll res = 0;
        if (ql <= mid) res = (res + query(p * 2, l, mid, ql, qr)) % mod;
        if (qr > mid) res = (res + query(p * 2 + 1, mid + 1, r, ql, qr)) % mod;
        return res;
    }
};
