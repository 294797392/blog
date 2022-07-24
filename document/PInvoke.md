## C#调用C++的函数的时候提示：找不到指定的模块/程序

出现这种情况一般都是程序运行根目录缺少依赖的dll导致，有几种办法可以查出来到底缺少了哪些dll：

1. 使用VS的dumpbin工具
dumpbin /dependents xxx.dll

2. 使用depends.exe工具

3. 使用msys环境下的ldd指令
ldd xxx.dll

需要注意的是，有可能被依赖的dll缺少依赖项，所以需要循环检查所依赖的每个dll