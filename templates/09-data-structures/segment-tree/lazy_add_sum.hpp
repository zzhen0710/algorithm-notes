#pragma once

#include <vector>

using ll = long long;

// a[1..n]，闭区间；区间加、区间和；所有中间乘积与和必须能用ll表示。
struct LazyAddSum {
    int n;
    std::vector<ll> tr, tag;

    explicit LazyAddSum(const std::vector<ll>& a)
        : n(static_cast<int>(a.size()) - 1), tr(4 * a.size()), tag(4 * a.size()) {
        build(1, 1, n, a);
    }

    void add(int l, int r, ll v) { add(1, 1, n, l, r, v); }
    ll query(int l, int r) { return query(1, 1, n, l, r); }

    void pull(int p) { tr[p] = tr[p * 2] + tr[p * 2 + 1]; }

    void build(int p, int l, int r, const std::vector<ll>& a) {
        if (l == r) { tr[p] = a[l]; return; }
        int mid = l + (r - l) / 2;
        build(p * 2, l, mid, a);
        build(p * 2 + 1, mid + 1, r, a);
        pull(p);
    }

    void apply(int p, int l, int r, ll v) {
        tr[p] += v * (r - l + 1); // 当前摘要立即生效。
        tag[p] += v;             // 孩子尚未接收的每项增量。
    }

    void push(int p, int l, int r) {
        if (l == r || tag[p] == 0) return;
        int mid = l + (r - l) / 2;
        apply(p * 2, l, mid, tag[p]);
        apply(p * 2 + 1, mid + 1, r, tag[p]);
        tag[p] = 0;
    }

    void add(int p, int l, int r, int ql, int qr, ll v) {
        if (ql <= l && r <= qr) { apply(p, l, r, v); return; }
        push(p, l, r);
        int mid = l + (r - l) / 2;
        if (ql <= mid) add(p * 2, l, mid, ql, qr, v);
        if (qr > mid) add(p * 2 + 1, mid + 1, r, ql, qr, v);
        pull(p);
    }

    ll query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p];
        push(p, l, r);
        int mid = l + (r - l) / 2;
        ll res = 0;
        if (ql <= mid) res += query(p * 2, l, mid, ql, qr);
        if (qr > mid) res += query(p * 2 + 1, mid + 1, r, ql, qr);
        return res;
    }
};
