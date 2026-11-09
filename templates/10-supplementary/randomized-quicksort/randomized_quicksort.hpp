#pragma once

#include <random>
#include <utility>
#include <vector>

inline void randomized_quick_sort(std::vector<int>& a, int l, int r, std::mt19937& rng) {
    if (l >= r) return;

    int pivot = a[std::uniform_int_distribution<int>(l, r)(rng)];
    int i = l, j = r;

    while (i <= j) {
        while (a[i] < pivot) ++i;
        while (a[j] > pivot) --j;
        if (i <= j) {
            std::swap(a[i], a[j]);
            ++i;
            --j;
        }
    }

    randomized_quick_sort(a, l, j, rng);
    randomized_quick_sort(a, i, r, rng);
}
