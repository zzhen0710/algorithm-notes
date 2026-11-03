# 第七章 数学：范式索引

博弈资源阈值、策略证明、递推矩阵建模与凸图形位置条件。对应[本章知识笔记](../../notes/07-math/README.md)。

## 本章范式

- [博弈中的临界条件：从资源阈值到策略证明](critical-condition-in-game.md)

- [从递推式构造矩阵：先留历史，再写每个分量的更新](recurrence-to-matrix.md)：依赖项、历史搬运、逐行提取系数、初始向量、转移次数与答案位置，补[已知初值递推与方程求解的区别](recurrence-to-matrix.md#已知初值向前推还是由约束求未知量)。

- [凸图形同侧判定：把内部写成半平面的交集](convex-halfplanes.md)：按边界方向逐边检查，区分严格内部、边界与非凸情形。

## 知识衔接

- [7.11 扩展欧几里得与一般模逆元](../../notes/07-math/11-extended-euclid.md)：从下一层等式恢复本层系数，再利用互质条件计算逆元。

- [7.12 基础几何](../../notes/07-math/12-basic-geometry.md#11-自查与衔接)：叉积与范围条件的组合；两题已静态复盘，并链接凸图形同侧范式及整数模板。

- [7.13 基础概率与期望](../../notes/07-math/13-probability-expectation.md)：[P4316](../../notes/07-math/13-probability-expectation.md#91-p4316直接把等概率出边接到模板)按后继期望反推，[CF621C](../../notes/07-math/13-probability-expectation.md#92-cf621c拆成相邻对的奖金贡献)按局部收益拆贡献；区分选择独立与期望相加，不另建重复模式。

## 跨章参考

- [MEX：从定义域与频率出发](../02-sequence-techniques/mex-analysis.md)
- [同一种线性变换重复很多次：矩阵快速幂](../09-data-structures/composable-transformations.md#同一种线性变换重复很多次矩阵快速幂)：用可复合的表示合并重复操作，对应 [7.9 矩阵笔记](../../notes/07-math/09-matrix-exponentiation.md)。

[返回全部范式](../README.md)
