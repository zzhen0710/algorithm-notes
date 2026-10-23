#pragma once

#include <algorithm>
#include <vector>

// 复制传入的值，排序去重；调用方的原数组不变
// 例如：{100,-5,100,7} -> {-5,7,100}
inline std::vector<long long> buildCoordinates(std::vector<long long> values) {
    std::sort(values.begin(), values.end());
    values.erase(std::unique(values.begin(), values.end()), values.end());
    return values;
}

// xs 必须升序且无重复，元素个数不超过 int 的表示范围
// 返回从 1 开始的编号；未收录时返回 -1，空表同样适用
inline int getId(const std::vector<long long>& xs, long long x) {
    auto it = std::lower_bound(xs.begin(), xs.end(), x);

    // 通用查询允许 x 未收录，需要确认第一个 >=x 的值就是 x
    // 若 xs 由完整原数组排序去重生成，且只查原数组中的值，此检查可省略
    // 模板仍保留它，以支持任意查询值和空表
    if (it == xs.end() || *it != x) return -1;

    // 编号从 1 开始，xs 的下标从 0 开始
    return static_cast<int>(it - xs.begin()) + 1;
}
