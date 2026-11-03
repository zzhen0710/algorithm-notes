#pragma once
#include <algorithm>
#include <cmath>
#include <vector>

// 整数平面几何；输入点坐标绝对值 <= 1e6，多边形顶点数 <= 200000。
// dot/cross 的向量可为上述点之间的差；不连续叠加向量后仍套用此界。
// 所有接口只读、无全局状态；整数判定精确，distance 返回浮点近似。
namespace geom {
using ll = long long;

struct Point {
    ll x, y;
    Point operator-(Point b) const { return {x - b.x, y - b.y}; }
};

inline ll dot(Point a, Point b) { return a.x * b.x + a.y * b.y; }
inline ll cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
inline int sgn(ll x) { return (x > 0) - (x < 0); }

// >0：c 在 a→b 左侧；<0：右侧；=0：共线（a、b 不同时）。
inline ll orient(Point a, Point b, Point c) {
    return cross(b - a, c - a);
}

inline ll dist2(Point a, Point b) {
    Point d = b - a;
    return dot(d, d); // 距离平方，比较远近时不必开方。
}

inline double distance(Point a, Point b) {
    return std::hypot((double)(a.x - b.x), (double)(a.y - b.y));
}

// 闭线段：端点也算；a==b 时只包含这一个点。
inline bool on_segment(Point p, Point a, Point b) {
    return orient(a, b, p) == 0
        && std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x)
        && std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}

inline bool segments_intersect(Point a, Point b, Point c, Point d) {
    ll c1 = orient(a, b, c), c2 = orient(a, b, d);
    ll c3 = orient(c, d, a), c4 = orient(c, d, b);

    // 先处理共线接触、重叠及退化为点的情况。
    if (c1 == 0 && on_segment(c, a, b)) return true;
    if (c2 == 0 && on_segment(d, a, b)) return true;
    if (c3 == 0 && on_segment(a, c, d)) return true;
    if (c4 == 0 && on_segment(b, c, d)) return true;

    // 排除边界后，两条线段必须相互跨过对方的直线。
    return sgn(c1) * sgn(c2) < 0 && sgn(c3) * sgn(c4) < 0;
}

// 非退化凸多边形，顶点沿边界有序、不重复；border 控制是否包含边界。
inline bool point_in_convex(const std::vector<Point>& poly, Point p, bool border = true) {
    int n = (int)poly.size(), side = 0;
    if (n < 3) return false;

    for (int i = 0; i < n; ++i) {
        int s = sgn(orient(poly[i], poly[(i + 1) % n], p));
        if (!s) {
            if (!border) return false;
            continue;
        }
        if (side && s != side) return false;
        side = s;
    }
    return side != 0;
}

// 简单多边形，顶点已沿边界顺序给出；返回非负的“两倍面积”。
inline ll polygon_area2(const std::vector<Point>& p) {
    int n = (int)p.size();
    if (n < 3) return 0;

    ll sum = 0;
    for (int i = 0; i < n; ++i)
        sum += cross(p[i], p[(i + 1) % n]);

    return sum >= 0 ? sum : -sum;
}
} // namespace geom
