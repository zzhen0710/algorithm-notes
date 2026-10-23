#pragma once

#include <algorithm>
#include <vector>

// Adapted from ei1333/library (Unlicense), revision 088b4c8b49f85194099305d998e7113dc3d5dd2e:
// https://github.com/ei1333/library/blob/088b4c8b49f85194099305d998e7113dc3d5dd2e/structure/union-find/union-find.hpp
// 1-based；合法编号为 1..n；0 只占位，操作要求编号合法
struct DSU {
    std::vector<int> p; // 根存负的集合大小；非根存父节点编号
    int components;

    explicit DSU(int n) : p(n + 1, -1), components(n) {
        p[0] = 0; // 0 不作为对象，也不计入集合数
    }

    int find(int x) {
        return p[x] < 0 ? x : (p[x] = find(p[x]));
    }

    // 返回是否真正合并两块，不返回根编号
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;

        if (p[a] > p[b]) std::swap(a, b); // 负得更多，集合更大
        p[a] += p[b]; // 两个负大小相加，得到新集合的负大小
        p[b] = a;
        --components;
        return true;
    }

    bool same(int a, int b) {
        return find(a) == find(b);
    }

    int size(int x) {
        return -p[find(x)];
    }
};
