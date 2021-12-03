# cygwin
参考：
https://www.cnblogs.com/findumars/p/8290405.html
https://github.com/msys2/msys2/wiki
https://blog.csdn.net/Com_ma/article/details/78682410

## cygwin

cygwin是一个虚拟的GNU运行环境，它实现了自己的一套CRT（cygwin1.dll），充当Linux API仿真层，提供大量的Linux下的函数。
使用cygwin编译出来的程序不依赖于msvcrt，依赖于cygwin1.dll。


## mingw

MinGW（简称“GNU for Windows”，全称“Minimalist GNU for Windows”）也是一个小型的GNU运行环境，它基于Win32API实现了部分Linux下的API（比如pthread）。
与cygwin相比，MinGW可以生成不依赖任何第三方CRT的Windows程序。
MinGW使用GCC编译器编译程序，所以所有GCC支持的编译器特性（比如typeof关键字），MinGW都支持。
MinGW是依赖于Win32 API实现的，不提供完整的POSIX API，所以在MinGW环境下没法使用比较高级的功能（例如fork，mmap，ioctl）。

## mingw和cygwin的区别

1. 文件路径


2. system函数的实现



## x86, x86-64, i386, IA64的区别
x86：Intel从16位微处理器8086开始的整个CPU芯片系列，系列中的每种型号都保持与以前的各种型号兼容，主要有8086，8088（16位CPU），80186，80286（这两个是过渡产品）， 80386，80486以及以后各种型号的Pentium芯片（32位CPU），通常所说的x86都是指32位CPU
i386：其实就是指Intel 80386，是第一个32位的x86架构的处理器，用了20多年了，之后的 486（80486，i486），586（80586，Pentium，P5），686（80686，Pentium Pro，P6）等等都与之兼容
x86-64：x86-64是x86指令集的超集，也就是64位CPU，在x86处理器上可以运行的程序也可以运行在x86-64上，因为目前我们说的64位的系统也只是在32位系统的基础上添加了一些64位的寻址功能。x86-64是AMD发明的，也叫AMD64（由于AMD的64位处理器产品线首先进入市场，且微软也不愿意为Intel和AMD开发两套不同的64位操作系统，Intel也被迫采纳AMD64指令集且增加某些新的扩充到他们自己的产品，命名为EM64T架构）
IA64：就是所谓的安腾，Intel跟HP联合折腾的一种64-bits全新架构，与x86系列不兼容，它放弃了以前的x86架构，认为它严重阻碍了处理器的性能提高。由于它不能很好地解决与以前32位应用程序的兼容，所以应用受到较大的限制，随着AMD Operon处理器的全面投入，Intel的IA64架构的处理器前景不容乐观
