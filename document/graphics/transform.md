# 矩阵变换

2D图像的变换原理就是对一张图像里的每一个像素点做一次矩阵的运算操作，运算后所得到的像素坐标点就是变换后的点。
旋转和缩放是使用坐标点乘变换矩阵，平移是坐标点加矩阵

下面列举了几个基本的变换矩阵。

注意：本文的变换都基于2D坐标系

### 旋转变换 - RotationTransform

设旋转角度为θ，那么有如下矩阵：

$$
\left[
    \begin{matrix}
        x & y \\ 
    \end{matrix}
\right]
\left[
    \begin{matrix}
        cos(θ) & sin(θ) \\ 
        -sin(θ) &  cos(θ) \\
    \end{matrix}
\right]
$$

X = x * cos(θ) - x * sin(θ) 
Y = y * sin(θ) + y * cos(θ)

### 缩放变换 - ScaleTransform

设x轴缩放倍数为sx，y轴缩放倍数为sy，那么有如下矩阵：

$$
\left[
    \begin{matrix}
        x & y \\ 
    \end{matrix}
\right]
\left[
    \begin{matrix}
    sx &  0 \\ 
    0  & sy \\
    \end{matrix}
\right]
$$

X = x * sx + x * 0
Y = y * 0 + y * sy


### 平移变换 - TranslationTramsform

假设x轴平移dx个距离，y轴平移dy个距离，那么平移矩阵就是：
$$
\left[
    \begin{matrix}
        x & y & 1 \\ 
    \end{matrix}
\right]
\left[
    \begin{matrix}
        1 & 0 & 0 \\ 
        0 & 1 & 0 \\
        dx & dy & 1 \\
    \end{matrix}
\right]
$$

X = x * 1 + y * 0 + 1 * dx
X = x + dx
Y = x * 0 + y * 1 + 1 * dy
Y = y + dy


注意，为了使用矩阵实现平移，这里把矩阵的变成了3X3的矩阵。
那为什么要使用矩阵实现平移呢？直接在X和Y坐标上加或减不是更方便吗？答案是使用3X3的矩阵可以同时实现缩放加平移或者旋转加平移。
而一个线性变化加一个平移变换的这种组合变换就叫做仿射变换。

### 仿射变换

仿射变换 = 线性变换 + 平移变换

使用3X3矩阵可以同时实现一个线性变化加一个平移变换

所以仿射变换就是把一个坐标点乘以一个3X3的矩阵
