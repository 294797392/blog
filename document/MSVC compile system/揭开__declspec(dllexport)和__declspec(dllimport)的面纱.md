## 简介

__declspec(dllexport)和__declspec(dllimport)属于函数修饰符，这两个修饰符会告诉编译器，该函数是要导出(export)的，或者是要从其他的库里的导入(import)的函数。

## 为什么需要__declspec(dllexport)和__declspec(dllimport)

我们先来看一个例子，





## __declspec(dllexport)
这个修饰符把函数定义为一个导出函数。
如果你在VisualStudio下编译纯C语言程序，编译器会自动给函数加上`__imp_`的修饰符前缀，编译出来的库文件(lib)里的符号名都以`__imp_xxxx`的形式存在。
外部程序在调用库文件的时候，需要给要调用的函数加上`__declspec(dllimport)`的修饰，

## __declspec(dllexport)
* C
__declspec(dllexport) void test();
* C++
class __declspec(dllexport) Test{};