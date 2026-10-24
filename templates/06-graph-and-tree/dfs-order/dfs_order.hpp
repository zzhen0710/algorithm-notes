#pragma once

#include <stack>
#include <vector>

// 1-based 连通无向树，固定合法根；只记录进入，子树区间为闭区间。
struct DFSOrder {
    std::vector<int> parent, depth, tin, tout, order;
    explicit DFSOrder(int n)
        : parent(n + 1), depth(n + 1), tin(n + 1), tout(n + 1), order(n + 1) {}

    int subtree_size(int u) const { return tout[u] - tin[u] + 1; }
    int kth_in_subtree(int u, int k) const { // 按本次 DFS 顺序，含 u；不存在返回 -1。
        if (k < 1 || k > subtree_size(u)) return -1;
        return order[tin[u] + k - 1]; // 先检查范围，再计算位置。
    }
    bool is_ancestor(int u, int v) const { // 包括 u==v。
        return tin[u] <= tin[v] && tin[v] <= tout[u];
    }
};

inline DFSOrder tree_order(const std::vector<std::vector<int>>& g, int root = 1) {
    int n = static_cast<int>(g.size()) - 1, timer = 0;
    DFSOrder t(n);
    std::vector<int> next(n + 1);
    std::stack<int> st;
    t.tin[root] = ++timer;
    t.order[timer] = root;
    st.push(root);
    while (!st.empty()) {
        int u = st.top();
        if (next[u] == static_cast<int>(g[u].size())) {
            t.tout[u] = timer; // 孩子已完成；退出不再增加 timer。
            st.pop();
            continue;
        }
        int v = g[u][next[u]++];
        if (v == t.parent[u]) continue;
        t.parent[v] = u;
        t.depth[v] = t.depth[u] + 1;
        t.tin[v] = ++timer;
        t.order[timer] = v;
        st.push(v);
    }
    return t;
}
