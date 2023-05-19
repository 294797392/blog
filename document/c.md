# C语言编程手册

## 常量指针
const char *p = "abcd";
指针所指向的存储空间（存储空间里的内容）不可修改
指针所指向的存储空间（存储空间里的内容）是一个常量
可以修改指针p的值，但是指针p指向的地址的内容（abcd）不可修改
指针p是一个常量（这个常量就是abcd）的指针


## 指针常量
char const *p = "abcd";
指针本身有const属性，不能修改指针p的值，但是可以修改指针p指向的地址的内容
指针p本身就是一个常量，所以叫做指针常量

https://blog.csdn.net/chenmozhe22/article/details/109738852


