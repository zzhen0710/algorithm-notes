#pragma once

#include <stack>
#include <vector>

using ll = long long;

// 0-based；返回右侧第一个严格更大元素的下标，不存在为 -1
inline std::vector<int> next_greater_right(const std::vector<ll>& a) {
    int n = static_cast<int>(a.size());
    std::vector<int> ans(n, -1);
    std::stack<int> st;

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && a[st.top()] < a[i]) {
            ans[st.top()] = i; // 当前新元素就是栈顶一直等待的答案
            st.pop();
        }
        st.push(i);
    }
    return ans;
}
