#ifndef __YFIRSTINCLUDE_H__
#define __YFIRSTINCLUDE_H__

// 封装判断不同平台的宏定义
// 这里封装一层的目的是为了方便引用不同平台的头文件, 因为有些编译环境的API和头文件是相同的，比方说Y_WIN32和Y_MINGW
// 把Y_ENV_WIN32和Y_ENV_MINGW封装成一个宏定义，那么这样后面再引用头文件的时候只要判断一个宏就可以了
// 那为什么还要定义Y_ENV_WIN32,Y_ENV_UNIX呢？
// 虽然有些平台的API和头文件是一样的，但是很多时候这些平台还是会有一些细微的不同的地方，可以通过这个宏来对这些地方做特殊处理
// 这些所有的宏也都可以通过cmake去配置，但是如果都通过cmake去配置的话，这样对于编译这个库就很麻烦，因为要记很多不同的宏定义
#if (defined(Y_ENV_WIN32)) || (defined(Y_ENV_MINGW))
#define Y_API_WIN32
#elif (defined(Y_UNIX))
#define Y_API_UNIX
#endif

#endif