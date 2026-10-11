继续：

# 10 · partition / stable_partition 与区间划分

这一节介绍 STL 中的**条件划分算法**。

它们的核心思想：

> 根据一个条件，将区间中的元素分成两部分。

例如：

给数组：

```text
1 4 3 6 5 2
```

条件：

```text
x % 2 == 0
```

划分：

```text
偶数区 | 奇数区
4 6 2 | 1 3 5
```

------

# 一、partition

## API

```cpp
template<class ForwardIt, class UnaryPredicate>
ForwardIt partition(
    ForwardIt first,
    ForwardIt last,
    UnaryPredicate pred
);
```

返回类型：

```cpp
iterator
```

作用：

> 将满足条件的元素放到前面，不满足条件的放到后面。

返回：

> 第一个不满足条件元素的位置。

------

例如：

```cpp
vector<int> a = {
    1,2,3,4,5,6
};

auto it =
    partition(
        a.begin(),
        a.end(),
        [](int x) {
            return x % 2 == 0;
        }
    );
```

结果可能：

```text
2 6 4 | 1 5 3
      ↑
      it
```

其中：

```text
[begin, it)
```

满足：

```cpp
x % 2 == 0
```

------

注意：

`partition()`：

只保证：

> 满足条件的在前面。

不保证：

> 原来的相对顺序。

例如：

原：

```text
2 1 4 3 6
```

划分偶数：

可能：

```text
6 2 4 1 3
```

偶数之间顺序改变。

------

# 二、stable_partition

## API

```cpp
template<class ForwardIt, class UnaryPredicate>
ForwardIt stable_partition(
    ForwardIt first,
    ForwardIt last,
    UnaryPredicate pred
);
```

作用：

> 划分的同时保持元素原来的相对顺序。

------

例如：

原：

```text
1 2 3 4 5 6
```

按偶数划分：

`partition`：

```text
6 2 4 1 5 3
```

不保证顺序。

------

`stable_partition`：

```text
2 4 6 | 1 3 5
```

保持：

偶数：

```text
2 4 6
```

奇数：

```text
1 3 5
```

------

# 三、partition 返回值

例如：

```cpp
auto it =
    partition(
        a.begin(),
        a.end(),
        pred
    );
```

假设：

```text
2 4 6 1 3 5
      ↑
      it
```

那么：

满足条件：

```text
[begin, it)
```

不满足：

```text
[it, end)
```

------

这个思想和：

- `lower_bound`
- `unique`
- `remove`

类似：

> 返回一个边界位置。

------

# 四、partition 常见应用

## 1. 快速划分

`partition` 的思想就是快速排序中的核心步骤。

快速排序：

选择 pivot：

```text
小于 pivot | pivot | 大于 pivot
```

本质：

也是一次划分。

------

## 2. 快速判断一类元素

例如：

把所有正数移动到前面：

```cpp
partition(
    a.begin(),
    a.end(),
    [](int x){
        return x > 0;
    }
);
```

------

## 3. 二分思想中的分组

很多问题：

需要：

```text
满足条件

↓

不满足条件
```

这种结构。

`partition` 就是直接构造这个结构。

------

# 五、partition 与 sort 区别

| 算法        | 作用         | 是否排序 |
| ----------- | ------------ | -------- |
| `partition` | 按条件分组   | ❌        |
| `sort`      | 按照规则排序 | ✅        |

例如：

```text
5 1 4 2 3
```

partition：

条件：

```cpp
x <= 3
```

结果：

```text
1 2 3 | 5 4
```

但是：

内部：

```text
1 2 3
```

不一定有序。

------

# 六、partition 与 stable_partition 区别

| 算法               | 是否保持相对顺序 | 空间         |
| ------------------ | ---------------- | ------------ |
| `partition`        | ❌                | 通常 $O(1)$  |
| `stable_partition` | ✅                | 需要额外空间 |

------

为什么 `stable_partition` 需要空间？

因为要保持：

```text
原顺序
```

例如：

原：

```text
1 2 3 4 5 6
```

偶数：

```text
2 4 6
```

如果原地交换，很容易破坏：

```text
2 4 6
```

的顺序。

因此通常需要额外缓冲区保存元素。

------

# 七、底层思想（了解）

`partition` 类似双指针：

```text
left
right
```

寻找：

左边不满足条件：

```text
bad
```

右边满足条件：

```text
good
```

交换：

```text
good ↔ bad
```

直到相遇。

------

手写：

```cpp
int l = 0;
int r = n - 1;

while (l <= r) {

    while (pred(a[l]))
        l++;

    while (!pred(a[r]))
        r--;

    if (l < r)
        swap(a[l++], a[r--]);
}
```

------

# 八、本节总结

| 算法               | 作用           | 返回       |
| ------------------ | -------------- | ---------- |
| `partition`        | 按条件划分区间 | `iterator` |
| `stable_partition` | 稳定划分区间   | `iterator` |

核心：

> partition 不负责排序，只负责根据条件形成两个区域。

使用：

```cpp
auto it =
    partition(
        begin,
        end,
        predicate
    );
```

得到：

```
满足条件 | 不满足条件
```

------

本节属于：

**理解型工具算法**

重点：

- 知道 `partition` 的作用；
- 理解返回 iterator 是边界；
- 理解稳定性区别。

不强求背底层实现。