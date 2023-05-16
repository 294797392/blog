# 不同类型的C运行库

* libucrt.lib：支持C99标准的CRT函数，在Windows10时代才开始作为Windows自带的组件。
* libvcruntime.lib：VisualC++ CRT实现  

以下的库里包含控制CRT初始化和反初始化的函数，这些库只能被静态链接到程序里
* libcmt.lib：Statically links the native CRT startup into your code
* libcmtd.lib：Statically links the Debug version of the native CRT startup. Not redistributable
* msvcrt.lib：Static library for the native CRT startup for use with DLL UCRT and vcruntime
* msvcrtd.lib：Static library for the Debug version of the native CRT startup for use with DLL UCRT and vcruntime. Not redistributable
* msvcmrt.lib：Static library for the mixed native and managed CRT startup for use with DLL UCRT and vcruntime.
* msvcmrtd.lib：Static library for the Debug version of the mixed native and managed CRT startup for use with DLL UCRT and vcruntime. Not redistributable

默认情况下，VisualStudio会链接libcmt.lib, libvcruntime.lib, libucrt.lib这三个库

CRT被封装在一个可在发行的DLL里，这个DLL就是MSVCRT.dll。可在发行的意思就是不同版本的VC++编译器使用的MSVCRT.dll版本也不同。
并且不同版本的VC++编译器对应的MSVCRT.dll的名字也是不一样的，例如MSVCRT10.dll or MSVCRT20.dll。
注意如果你的程序是多线程程序，那么一定要使用多线程版本的CRT，不然可能会出现重大问题。

# 使用CRT库来构建dll

在构建dll的时候，有两种方式可以确保CRT被正确的初始化了：
1. 入口点函数名设置成DllMainCRTStartup，然后再dll里新建一个函数，函数名字叫做DllMain。在Dll第一次被加载到进程空间中的时候，系统会首先自动调用DllMainCRTStartup函数，它会调用_CRT_INIT函数对CRT进行初始化，然后CRT_INIT函数会调用DllMain函数（如果存在的话）通知程序员，程序员可以在DllMain函数里做一些上层的数据初始化操作。
2. 自定义Dll的入口点函数，并在入口点函数显式调用_CRT_INIT函数。

在VisualStudio里默认使用的是第一种方式来初始化CRT，当入口点函数没有设置的时候，默认使用DllMainCRTStartup函数。  

如果你想使用第二种方式初始化CRT，有两种做法：
1. 省略，基本用不到这种方式
2. 自定义一个Dll入口点，然后遵循以下步骤：  
    1. 在代码里导入初始化CRT的函数的原型：extern BOOL WINAPI _CRT_INIT(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);
    2. 在DLL_PROCESS_ATTACH和DLL_THREAD_ATTACH事件触发的时候，调用_CRT_INIT函数
    3. 调用你自己的初始化数据的函数
    4. 在DLL_PROCESS_DETACH和DLL_THREAD_DETACH事件触发的时候，再次调用_CRT_INIT函数来释放CRT库

示例代码如下：
```C

#include <Windows.h>

extern BOOL WINAPI _CRT_INIT(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved);

BOOL WINAPI DllEntryPoint(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH || fdwReason == DLL_THREAD_ATTACH)
    if (!_CRT_INIT(hinstDLL, fdwReason, lpReserved))
    return(FALSE);

    if (fdwReason == DLL_PROCESS_DETACH || fdwReason == DLL_THREAD_DETACH)
    if (!_CRT_INIT(hinstDLL, fdwReason, lpReserved))
    return(FALSE);
    return(TRUE);
}

```


> 本文翻译自：  
https://learn.microsoft.com/en-us/troubleshoot/developer/visualstudio/cpp/libraries/use-c-run-time
https://learn.microsoft.com/en-us/cpp/c-runtime-library/crt-library-features?view=msvc-170
