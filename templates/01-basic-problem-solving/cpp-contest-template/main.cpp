#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

// 常用显式类型转换：按需取用，先确认目标类型能容纳原值
// int n = static_cast<int>(a.size());             // 容器长度转 int
// int pos = static_cast<int>(it - a.begin());     // 迭代器差值转下标
// ll product = static_cast<ll>(x) * y;            // 先扩大类型，再乘法
// double avg = static_cast<double>(sum) / cnt;    // cnt 非零，使用小数除法
// int code = static_cast<unsigned char>(c) + 1;   // 字节按非负值编码
// static_cast 不检查范围；double 转 int 向 0 截断，不能补救此前的溢出

void solve() {

}

int main() {
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
#endif

    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T = 1;
    // cin >> T;

    while (T--) {
        solve();
    }

    return 0;
}