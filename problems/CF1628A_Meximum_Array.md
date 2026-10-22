# CF 1628A — Meximum Array

题目：[Codeforces 1628A](https://codeforces.com/problemset/problem/1628/A)。分类：贪心、MEX、前缀收集、后缀预处理。

收录原因：理解如何用未来信息辅助当前决策，并区分“能够达到的目标”与“已经满足的条件”；在贪心分段中同时关注当前收益和保留后续选择。

## 题意与核心思想

每次删除当前数组的一个**非空前缀**，把它的 MEX 加入答案，最终使答案数组的**字典序最大**。相同前缀下，较长的答案更大。约束：`1 <= n <= 200000`、`0 <= a[i] <= n`，所有测试的 `n` 之和不超过 `200000`。

`mex = k` 表示 `0..k-1` 全部出现，`k` 没出现；它描述的是缺失关系，不是不同数字的数量。

当前剩余后缀的 MEX 为 `k`，任何前缀都不可能获得更大的 MEX。因此本段先争取 `k`，再选择**收齐 `0..k-1` 的最短非空前缀**。同样输出 `k` 时，早点切割会留下更多元素；后续仍可把多留的元素并入下一段，不会让该段 MEX 变小。这就是本题的贪心边界。

## 解法一：剩余频率与前缀收集（个人 AC）

两份代码保留原逻辑，仅统一排版并增加注释。

先统计剩余元素频率；每段从 `0` 找出当前 MEX，再向右收集目标值。`need` 只在一个小于目标 MEX 的值第一次出现时减少，消费任何元素都要扣除剩余频率。收齐后立即切割，不实际删除数组。

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

void solve() {
    int n;
    cin >> n;
    vector<int> a(n);
    vector<int> b;
    unordered_map<int, int> cnt; // 当前未处理后缀的频率
    for (int& v : a) {
        cin >> v;
        ++cnt[v];
    }
    for (int i = 0; i < n;) {
        int j = 0;
        while (cnt[j]) ++j; // 从 0 找第一个缺失值；缺失键默认插入为 0
        if (!j) { // 后缀没有 0：每个剩余元素单独成段
            while (i < n) {
                b.push_back(0);
                i++;
            }
            break;
        }
        int need = j; // 还需收齐的目标值数量，不是任意不同值的数量
        vector<int> vis(j, 0);
        while (need > 0) {
            int x = a[i];
            if (x < j && !vis[x]) { // 只统计 0..j-1 的首次出现
                vis[x] = 1;
                --need;
            }
            --cnt[x]; // 消费元素后同步剩余频率，包括重复值和大值
            ++i;
        }
        b.push_back(j); // need 归零时立刻切割，当前段最短
    }
    cout << b.size() << '\n';
    for (int v : b) cout << v << ' ';
    cout << '\n';
}

int main() {
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
#endif
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T = 1;
    cin >> T;
    while (T--) solve();
    return 0;
}
```

复杂度：在哈希操作平均常数时间的前提下，总时间为 `O(n)`，空间为 `O(n)`。每段目标为 `k` 时至少消费 `k` 个元素，因此所有段的目标之和不超过 `n`；反复从 `0` 扫 MEX 和初始化 `vis(k)` 的累计工作也是线性的。`unordered_map` 不提供相同的最坏时间保证；`cnt[j]` 查询缺失键时会插入零值，这是本实现的语义。

## 踩坑一：把 MEX 当作任意不同数字的数量

错误方向：

```cpp
// 错误示意：任意凑够 mex 个不同值，并不代表得到该 mex。
while (seen.size() < mex) {
    seen.insert(a[i++]);
}
```

剩余数组 `[5,6,7,0,1,2]` 的 MEX 是 `3`，但前三个不同数字 `[5,6,7]` 的 MEX 是 `0`。目标必须是 `0、1、2`，不是随意三个不同值。

修正就是代码里的 `x < target && !found[x]`：只收集目标范围内首次出现的数。重复目标值、大于目标的值都不能减少 `need`。

## 踩坑二：MEX 为 0 时仍须消费元素

当前剩余部分没有 `0`，之后任何段的 MEX 都只能是 `0`。应把每个剩余元素单独成段，得到尽可能长的全零后缀；不是把所有剩余元素合成一段只输出一个 `0`。

例如 `[2,2,1]` 应输出 `[0,0,0]`。若直接令 `need=0`，跳过收集循环却不移动指针，就可能死循环。两份代码都用单独分支保证推进；也可以统一成“每段至少消费一个元素”的实现，此处保留原写法。

## 解法二：后缀 MEX 记录（题解思路复现）

预处理 `surf[i] = mex(a[i..n-1])`。从右往左只加入元素，预处理中的 `mex` 只增不减；正向切分时直接读取段起点的目标，再收集实现它的最短前缀。

```cpp
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;

void solve() {
    int n; cin >> n;
    vector<int> a(n);
    for (int& v : a) cin >> v;
    vector<bool> record(n + 2); // 构建后缀时，记录已经出现的值
    vector<int> surf(n); // surf[i] = mex(a[i..n-1])，保留原变量名
    int mex = 0;
    for (int i = n - 1; i >= 0; --i) {
        record[a[i]] = true;
        while (record[mex]) ++mex; // 从右向左只增加元素，mex 只增不减
        surf[i] = mex;
    }
    vector<int> b;
    for (int i = 0; i < n;) {
        int get = surf[i]; // 固定本段目标；不要在收集过程中随 i 改变
        if (get == 0) { // 单元素切段，保留尽可能多的 0
            while (i < n) {
                i++;
                b.push_back(0);
            }
            break;
        }
        b.push_back(get);
        int need = get;
        vector<bool> found(need); // 新段独立记录目标值，初始全为 false
        while (need > 0) {
            int x = a[i];
            if (x < get && !found[x]) { // 短路判断避免访问 found 的范围外
                found[x] = true;
                --need;
            }
            ++i; // 每次消费一个元素，收齐即停止
        }
    }
    cout << b.size() << '\n';
    for (int v : b) cout << v << ' ';
    cout << '\n';
}

int main() {
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
#endif
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T = 1;
    cin >> T;
    while (T--) solve();
    return 0;
}
```

时间、空间均为 `O(n)`。反向扫描的 MEX 指针最多增长到 `n`；正向指针只走一遍，各段 `found` 的总初始化长度不超过 `n`。`record` 保存整段后缀的出现情况，`found` 只保存本段已收集的目标值，二者不能混用。

该实现是题解思路的个人复现，**不是官方代码原文**。[官方题解入口](https://codeforces.com/blog/entry/99276)在整理核验时仅返回加载占位，正文未核验。

## 两种做法的关系与进步空间

| 比较 | 初 AC | 后缀版 |
| --- | --- | --- |
| 如何知道本段目标 | 维护剩余频率，每段求 MEX | 提前保存每个后缀的 MEX |
| 如何找到切点 | 收齐 `0..k-1` 后立即停止 | 同样收齐 `0..k-1` 后停止 |
| 状态维护 | 消费元素时递减频率 | 切分阶段只读取后缀记录 |
| 总时间 | 哈希平均意义下 `O(n)` | 数组实现 `O(n)` |
| 改进收益 | 自然对应“剩余数组” | 利用已知值域，省去哈希查询与正向频率维护，目标更直观 |

两版的贪心模型相同，后缀版改善的是状态获取方式，**不能把初版误写成每段重扫剩余数组的 `O(n²)` 算法**。也可以把初版频率表换成数组，得到另一种确定性线性实现；后缀预处理不是唯一完善方案。

如何想到：模拟删除前缀 → 用指针表示剩余数组 → 发现每次只问剩余后缀的 MEX → 数组本身不变、起点单调右移 → 从右往左提前记录所有起点的信息 → 正向只负责收集与切分。

特别注意：**后缀状态变化不等于合法切点。** `[0,1]` 的后缀 MEX 为 `[2,0]`，只看下降就可能在 `0` 后切割，但该段 MEX 只有 `1`。目标固定为段起点的 `surf[start]`，必须等本段收齐它要求的全部值。

## 最终模型与关联

本题可以压缩为：**后缀供给决定本段能达到的 MEX，前缀收集决定最早在哪里达到。** 提高当前答案由字典序要求决定；达到目标后尽早切割由保留后续选择决定。不能泛化成“分段贪心都不必考虑未来”。

- [MEX：从定义域与频率出发](../patterns/02-sequence-techniques/mex-analysis.md)：先确定必须覆盖的连续值域。
- [后缀预处理：用未来摘要辅助当前决策](../patterns/02-sequence-techniques/suffix-preprocessing.md)：当前决策反复依赖右侧时，提前保存所需信息。

## 核验记录

两份实现均按已有 AC 结果记录；平台提交记录尚未独立核验。两份原逻辑均以 C++17、`-O2 -Wall -Wextra` 编译，并通过原题 6 组样例、21,835 个小数组与独立穷举切分结果的对拍，以及 4 组 `n=200000` 边界：全 `0`、全 `n`、`0..n-1`、交替 `0/1`。

小数组覆盖 `1 <= n <= 7`、`0 <= a[i] <= min(n,3)` 的全部数组；有限测试不代替贪心依据。代码保留原 `surf` 命名、局部去重容器与 `mex=0` 分支，仅排版与注释作了整理。
