#pragma once

#include <algorithm>
#include <limits>
#include <vector>

using ll = long long;

// a[1..n]，闭区间；点赋值与区间最大值，n>=1。
struct SegMax {
    int n;
    std::vector<ll> tr;

    explicit SegMax(const std::vector<ll>& a)
        : n(static_cast<int>(a.size()) - 1), tr(4 * a.size()) {
        build(1, 1, n, a);
    }

    void set(int x, ll v) { set(1, 1, n, x, v); }
    ll query(int l, int r) const { return query(1, 1, n, l, r); }

    void pull(int p) { tr[p] = std::max(tr[p * 2], tr[p * 2 + 1]); }

    void build(int p, int l, int r, const std::vector<ll>& a) {
        if (l == r) { tr[p] = a[l]; return; }
        int mid = l + (r - l) / 2;
        build(p * 2, l, mid, a);
        build(p * 2 + 1, mid + 1, r, a);
        pull(p);
    }

    void set(int p, int l, int r, int x, ll v) {
        if (l == r) { tr[p] = v; return; }
        int mid = l + (r - l) / 2;
        if (x <= mid) set(p * 2, l, mid, x, v);
        else set(p * 2 + 1, mid + 1, r, x, v);
        pull(p);
    }

    ll query(int p, int l, int r, int ql, int qr) const {
        if (ql <= l && r <= qr) return tr[p];
        int mid = l + (r - l) / 2;
        ll res = std::numeric_limits<ll>::lowest();
        if (ql <= mid) res = std::max(res, query(p * 2, l, mid, ql, qr));
        if (qr > mid) res = std::max(res, query(p * 2 + 1, mid + 1, r, ql, qr));
        return res;
    }
};
