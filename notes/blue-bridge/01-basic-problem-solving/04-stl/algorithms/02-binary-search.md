# 02 · lower_bound / upper_bound / 二分查找接口

## 一、二分查找的核心思想

二分查找并不只是"寻找某个数字"。

它真正利用的是：

> **有序序列中的单调性，通过不断缩小范围寻找边界位置。**

例如：

``` text
满足条件        不满足条件

true true true | false false
                ↑
             边界位置
```

`lower_bound`、`upper_bound` 本质都是在寻找这样的边界。

前提：

-   区间必须已经按照对应规则有序。

复杂度：

-   随机访问迭代器：

$O(\log n)$

------------------------------------------------------------------------

# 二、lower_bound

## API

``` cpp
template<class ForwardIt, class T>
ForwardIt lower_bound(
    ForwardIt first,
    ForwardIt last,
    const T& value
);
```

返回类型：

``` cpp
iterator
```

作用：

> 返回第一个 **大于等于 value** 的元素位置。

即：

$>= value$

------------------------------------------------------------------------

例：

``` cpp
vector<int> a = {1,3,3,3,5,8};

auto it = lower_bound(
    a.begin(),
    a.end(),
    3
);
```

结果：

``` text
1 3 3 3 5 8
  ↑
```

返回第一个 `3`。

------------------------------------------------------------------------

如果查找：

``` cpp
lower_bound(
    a.begin(),
    a.end(),
    4
);
```

结果：

``` text
1 3 3 3 5 8
        ↑
```

返回 `5`。

------------------------------------------------------------------------

如果不存在：

``` cpp
auto it = lower_bound(
    a.begin(),
    a.end(),
    10
);
```

返回：

``` text
a.end()
```

因此使用前需要判断：

``` cpp
if (it != a.end()) {
    cout << *it;
}
```

------------------------------------------------------------------------

# 三、upper_bound

## API

``` cpp
template<class ForwardIt, class T>
ForwardIt upper_bound(
    ForwardIt first,
    ForwardIt last,
    const T& value
);
```

返回类型：

``` cpp
iterator
```

作用：

> 返回第一个严格大于 value 的元素位置。

即：

$> value$

------------------------------------------------------------------------

例：

``` cpp
vector<int> a = {1,3,3,3,5,8};

auto it = upper_bound(
    a.begin(),
    a.end(),
    3
);
```

结果：

``` text
1 3 3 3 5 8
        ↑
```

返回 `5`。

------------------------------------------------------------------------

# 四、lower_bound 与 upper_bound 的应用

## 1. 查找出现次数

对于有序数组：

``` cpp
vector<int> a = {1,3,3,3,5,8};
```

统计 `3`：

``` cpp
int cnt =
    upper_bound(
        a.begin(),
        a.end(),
        3
    )
    -
    lower_bound(
        a.begin(),
        a.end(),
        3
    );
```

结果：

``` text
3
```

原因：

``` text
1 3 3 3 5 8
  ↑     ↑
 lower upper
```

两者之间正好覆盖所有 `3`。

------------------------------------------------------------------------

## 2. 小于 x 的数量

``` cpp
lower_bound(begin, end, x) - begin
```

因为：

`lower_bound` 返回第一个 ≥ x 的位置。

之前的所有元素：

$<x$

------------------------------------------------------------------------

## 3. 小于等于 x 的数量

``` cpp
upper_bound(begin, end, x) - begin
```

因为：

`upper_bound` 返回第一个 \> x 的位置。

之前的所有元素：

$<=x$

------------------------------------------------------------------------

# 五、binary_search

## API

``` cpp
template<class ForwardIt, class T>
bool binary_search(
    ForwardIt first,
    ForwardIt last,
    const T& value
);
```

返回类型：

``` cpp
bool
```

作用：

> 判断有序区间中是否存在 value。

------------------------------------------------------------------------

例：

``` cpp
vector<int> a = {1,3,5,7,9};

bool ok = binary_search(
    a.begin(),
    a.end(),
    5
);
```

返回：

``` text
true
```

------------------------------------------------------------------------

与 `lower_bound` 区别：

  需求         方法
------------ -----------------------------
  只判断存在   `binary_search`
  需要位置     `lower_bound`
  需要数量     `upper_bound - lower_bound`

竞赛中：

`lower_bound` 使用频率更高，因为很多问题需要进一步操作位置。

------------------------------------------------------------------------

# 六、equal_range

## API

``` cpp
template<class ForwardIt, class T>
pair<ForwardIt, ForwardIt>
equal_range(
    ForwardIt first,
    ForwardIt last,
    const T& value
);
```

返回类型：

``` cpp
pair<iterator, iterator>
```

作用：

返回：

``` cpp
{
    lower_bound(value),
    upper_bound(value)
}
```

------------------------------------------------------------------------

例：

``` cpp
vector<int> a = {
    1,3,3,3,5,8
};

auto range =
    equal_range(
        a.begin(),
        a.end(),
        3
    );
```

结果：

``` text
1 3 3 3 5 8
  ↑     ↑
 first second
```

其中：

``` cpp
range.first
```

指向第一个 `3`。

``` cpp
range.second
```

指向 `5`。

------------------------------------------------------------------------

统计数量：

``` cpp
auto range =
    equal_range(
        a.begin(),
        a.end(),
        x
    );

int cnt =
    range.second - range.first;
```

------------------------------------------------------------------------

# 七、自定义比较器版本

如果排序规则改变：

二分函数也必须使用相同规则。

例如：

降序数组：

``` cpp
vector<int> a = {
    9,7,5,3,1
};
```

使用：

``` cpp
greater<int>()
```

定义排序规则：

``` cpp
auto it =
    lower_bound(
        a.begin(),
        a.end(),
        6,
        greater<int>()
    );
```

结果：

``` text
9 7 5 3 1
    ↑
```

找到第一个满足降序规则的位置。

------------------------------------------------------------------------

注意：

错误：

``` cpp
sort(
    a.begin(),
    a.end(),
    greater<int>()
);

lower_bound(
    a.begin(),
    a.end(),
    x
);
```

排序规则与二分规则不一致，会导致错误结果。

------------------------------------------------------------------------

# 八、algorithm 与容器成员函数区别

## 1. set / map

推荐：

``` cpp
set<int> s;

auto it = s.lower_bound(x);
```

原因：

关联容器底层维护平衡树。

复杂度：

$O(\log n)$

------------------------------------------------------------------------

## 2. vector / array

使用：

``` cpp
lower_bound(
    a.begin(),
    a.end(),
    x
);
```

因为序列容器没有成员函数。

------------------------------------------------------------------------

注意：

不要：

``` cpp
lower_bound(
    s.begin(),
    s.end(),
    x
);
```

虽然可以编译，但是对于 `set`：

-   迭代器不是随机访问；
-   algorithm 二分移动效率较低。

因此：

> 关联容器优先使用成员函数，序列容器使用 algorithm。

------------------------------------------------------------------------

# 九、代表题

## P2249 查找

### 思路

数组已经升序。

询问数字 x：

使用：

``` cpp
lower_bound()
```

找到第一个：

$>=x$

的位置。

判断：

``` cpp
it != end()
&&
*it == x
```

即可。

复杂度：

$O(n+m\log n)$

------------------------------------------------------------------------

## P1678 烦恼的高考志愿

### 思路

学校分数线排序：

``` cpp
sort(a.begin(), a.end());
```

对于每个学生：

``` cpp
lower_bound()
```

寻找第一个：

$>= 当前成绩$

的学校。

答案只可能在：

-   当前学校；
-   前一个学校。

比较距离即可。

核心思想：

> 二分不是直接寻找答案，而是寻找答案附近的候选位置。

------------------------------------------------------------------------

# 十、二分接口选择总结

| 需求              | API                         |
| ----------------- | --------------------------- |
| 第一个 ≥ x        | `lower_bound`               |
| 第一个 > x        | `upper_bound`               |
| 判断是否存在      | `binary_search`             |
| 寻找所有 x 的范围 | `equal_range`               |
| 统计 x 出现次数   | `upper_bound - lower_bound` |
| 统计 < x 数量     | `lower_bound - begin`       |
| 统计 ≤ x 数量     | `upper_bound - begin`       |

## 核心思想总结

STL 二分接口的本质：

> **在满足单调性的序列中寻找边界，而不是简单查找元素。**

理解边界，比记 API 更重要。

后续：

-   二分答案；
-   贪心中的位置确定；
-   离散化；

都会继续使用这一思想。
