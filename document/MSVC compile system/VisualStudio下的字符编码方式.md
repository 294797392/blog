# VC++开发环境下的多字节字符集/Unicode字符集编码方式的区别

# Unicode字符集
Unicode通常用两个字节表示一个字符，原有的英文编码从单字节变成双字节，只需要把高字节全部填为0就可以

# 字符的数据类型不同
unicode字符集编码方式使用wchar_t表示一个字符，占用两个字节的空间，用wchar_t类型定义的字符又叫做宽字符，用多个字节来存储的字符叫做宽字符。
多字节字符编码方式使用char表示一个字符，占用一个字节的空间，用char类型定义的字符又叫做窄字符。
所以wchar_t类型可以存储任意字符，包括汉字和其他国家的文字，而char只能存储ascii码和一些其他特殊字符。
在定义wchar_t类型的字符串的时候，需要在字符串前面加L，以便告诉编译器该字符串里的每个字符都是宽字符，每个字符占用2字节的空间。
为了在开发的时候适配宽字符和窄字符，微软定义了如下宏：
````
#ifdef UNICODE
#define __TEXT(quote) L##quote
typedef wchar_t TCHAR;
#else
#define __TEXT(quote)
typedef char TCHAR;
#endif
````

# 字符串操作函数不同
unicode字符集编码方式使用wprintf输出字符串
多字节字符编码方式使用printf输出字符串

# API函数不同
使用unicode字符集编码方式的API后缀是W，并且函数参数里所有的字符串类型都是TCHAR。
多字节字符编码方式的API后缀是A，并且函数参数里所有的字符串类型都是char。
为了适配unicode字符集和多字节字符集，微软为每个API函数都定义了一个把函数后缀去掉了的宏。

# 宽字符和多字节字符的转换
多字节字符转宽字符：mbstowcs
宽字符转多字节字符：wcstombs
