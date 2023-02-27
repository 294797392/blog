# WPF中的2D坐标矩阵变换

2D图像的变换原理就是对一张图像里的每一个像素点乘一个3x3的矩阵，运算后所得到的像素坐标点就是变换后的点。
2D图像的变换有三种，分别是旋转，缩放和平移，每种变换都对应着一个变换矩阵。

下面列举了几个基本的变换矩阵。

## 旋转变换 - RotationTransform

angle：旋转角度
centerX：旋转中心点X坐标
centerY：旋转中心点Y坐标

$$
\left[
    \begin{matrix}
        x & y & 1 \\ 
    \end{matrix}
\right]
\left[
    \begin{matrix}
        cos(angle) & sin(angle) & 0 \\ 
        -sin(angle) & cos(angle) & 0 \\
        centerX * (1 - cos(angle) + centerY * sin(angle)) &  centerY * (1 - cos(angle) - centerY * sin(angle)) & 1 \\
    \end{matrix}
\right]
$$

__X = x * cos(θ) - y * sin(θ) + 1 * 0__  
__Y = x * sin(θ) + y * cos(θ) + 1 * 0__

## 缩放变换 - ScaleTransform

scaleX：X轴缩放倍数
scaleY：Y轴缩放倍数
centerX：缩放中心点X坐标
centerY：缩放中心点Y坐标

$$
\left[
    \begin{matrix}
        x & y & 1 \\ 
    \end{matrix}
\right]
\left[
    \begin{matrix}
    scaleX &  0 & 0 \\ 
    0  & scaleY & 0 \\
    centerX - scaleX * centerX  & centerY - scaleY * centerY & 1 \\
    \end{matrix}
\right]
$$

__X = x * scaleX + y * 0 + 1 * (centerX - scaleX * centerX)__  
__Y = x * 0 + y * scaleY + 1 * (centerY - scaleY * centerY)__


## 平移变换 - TranslationTramsform

offsetX：X平移量
offsetY：Y平移量

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
        offsetX & offsetY & 1 \\
    \end{matrix}
\right]
$$

__X = x * 1 + y * 0 + 1 * offsetX__  
__Y = x * 0 + y * 1 + 1 * offsetY__

## 仿射变换

仿射变换 = 平移/缩放变换 + 平移变换  
使用3X3矩阵可以同时实现一个线性变化加一个平移变换  
所以仿射变换就是把一个坐标点乘以一个3X3的矩阵


## 逆矩阵的作用
假设有一个点P1，有一个变换矩阵A，对点P1进行矩阵变换得到点P2：
P1 * A = P2

有的时候需要根据P2和矩阵A去求变换前的坐标点P1，那么公式变成如下：
P1 = P2 / A
但是矩阵是没有除法的，此时可以用矩阵A的逆矩阵来算出P1。
P1 * A * A-1 = P2 * A-1
P1 * I = P2 * A-1
P1 = P2 * A-1
通过矩阵A的逆矩阵就求出来了变换前的坐标P1。


























