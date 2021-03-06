# C头文件的问题

最近在用QTDesigner的时候，发现QTDesigner自动生成的头文件里既包含了函数声明也包含了实现，突然想到平时我们写C/C++的时候为什么不直接把声明和实现都写在头文件里呢，这样不是更方便吗？思考了一下，发现头文件里只写函数声明还是有一定道理的，现在总结一下。

1. 函数的复用性
我们在写一些可复用函数的时候，通常会在多个C文件里去#include这个函数的头文件，如果把函数的实现直接写在头文件里，那么对链接器来说，会有同一个函数的多个实现，链接器在连接的时候就不知道使用哪个函数了，这种情况下编译会出问题。
如果头文件里只写了声明，那么即使#include了多次，编译的时候也不会出错，因为C语言可以对一个函数进行多次声明，但是只能有一个实现，这个是链接器的特性。
QTDesigner之所以会把实现也写在头文件里，因为QTDesigner生成的是布局文件，可以保证只有一个类去#include它，所以这样做是完全没有问题的。
