## 文件中段

exe或者dll文件中里的数据以**段**的形式来保存。在Windows平台下，段叫做section，linux平台下，段叫做segement。
下面列举一些常见的段：
数据段（.data）：保存程序中所用到的变量数据
常量段（.rdata）：保存程序中定义的常量数据
代码段（.text）：保存程序中用来执行的二进制代码
调试段（.debug）：保存和调试相关的数据，如果你的可执行文件是使用debug模式生成的，那么一般情况下都会有这个段存在

这些段都是我们在编译程序的时候编译器自动生成的，MSVC的编译器给我们提供了一个预处理指令，可以让我们自定义段，通过自定义段可以实现一些特殊的操作。
这里主要介绍下自定义数据段（.data）的方式。

## 自定义数据段

使用**#pragma section("section_name" [ , attributes ])**预处理指令来定义一个数据段。
**section_name**指定数据段的名字，attribute指定了数据段的属性，attribute使用了中括号括起来，表示attribute是可选的参数。
下面列举了attribute参数可能的值：
Attribute	Description
read	Allows read operations on data.
write	Allows write operations on data.
execute	Allows code to be executed.
shared	Shares the section among all processes that load the image.
nopage	Marks the section as not pageable. Useful for Win32 device drivers.
nocache	Marks the section as not cacheable. Useful for Win32 device drivers.
discard	Marks the section as discardable. Useful for Win32 device drivers.
remove	Marks the section as not memory-resident. For virtual device drivers (VxD) only.
如果这个段拥有多个属性，那么属性之间使用逗号隔开。
如果不指定attribute参数，那么编译器会自动使用read和write。

默认情况下，MSVC编译器会把我们在程序里定义的数据（变量）存储到.data段里。
如果要把数据存储到自定义段里，那么就需要使用**__declspec(allocate("section_name"))**指令来告诉编译器，把数据保存到section_name段里。

下面给出示例代码：

`
#pragma section("mysec",read,write)
int j = 0;

__declspec(allocate("mysec")) int i = 0;

int main(){}
`

上面的代码里，使用**#pargma section("mysec",read,write)**定义了名字为mysec的数据段，并且该段的属性为可读（read），可写（write），
然后定义了一个变量j，在编译的时候，因为j没有使用**__declspec(allocate("mysec"))**来修饰，所以编译器默认会把它放到.data段里进行存储，
随后定义了一个变量i，变量i使用了**__declspec(allocate("mysec"))**来修饰，所以i这个变量就存储在mysec这个数据段里。
注：上面的例子摘抄自MSDN（https://docs.microsoft.com/en-us/cpp/preprocessor/section?view=msvc-170）








