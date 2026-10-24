#pragma once

#include <string>
#include <vector>

// 输入s为普通0-based非空01串；操作端点为1-based闭区间。
struct LazyFlipCount {
    int n;
    std::vector<int> tr, tag;

    explicit LazyFlipCount(const std::string& s)
        : n(static_cast<int>(s.size())), tr(4 * s.size()), tag(4 * s.size()) {
        build(1, 1, n, s);
    }

    void flip(int l, int r) { flip(1, 1, n, l, r); }
    int query(int l, int r) { return query(1, 1, n, l, r); }

    void pull(int p) { tr[p] = tr[p * 2] + tr[p * 2 + 1]; }

    void build(int p, int l, int r, const std::string& s) {
        if (l == r) { tr[p] = s[l - 1] - '0'; return; }
        int mid = l + (r - l) / 2;
        build(p * 2, l, mid, s);
        build(p * 2 + 1, mid + 1, r, s);
        pull(p);
    }

    void apply(int p, int l, int r) {
        tr[p] = r - l + 1 - tr[p];
        tag[p] ^= 1; // 两次翻转抵消，只保留奇偶。
    }

    void push(int p, int l, int r) {
        if (l == r || tag[p] == 0) return;
        int mid = l + (r - l) / 2;
        apply(p * 2, l, mid);
        apply(p * 2 + 1, mid + 1, r);
        tag[p] = 0;
    }

    void flip(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) { apply(p, l, r); return; }
        push(p, l, r);
        int mid = l + (r - l) / 2;
        if (ql <= mid) flip(p * 2, l, mid, ql, qr);
        if (qr > mid) flip(p * 2 + 1, mid + 1, r, ql, qr);
        pull(p);
    }

    int query(int p, int l, int r, int ql, int qr) {
        if (ql <= l && r <= qr) return tr[p];
        push(p, l, r);
        int mid = l + (r - l) / 2;
        int res = 0;
        if (ql <= mid) res += query(p * 2, l, mid, ql, qr);
        if (qr > mid) res += query(p * 2 + 1, mid + 1, r, ql, qr);
        return res;
    }
};
