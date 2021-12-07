#ifndef __YBASE_H__
#define __YBASE_H__

#include "Yfirstinclude.h"

#ifdef Y_API_WIN32
#include <Windows.h>
#elif Y_API_UNIX
#endif

// 定义导出和导入符号
#ifdef Y_ENV_WIN32
    #ifdef Y_EXPORT
        #define YAPI __declspec(dllexport)
    #else
        #define YAPI __declspec(dllimport)
    #endif
#else
    #define YAPI
#endif

// 起个别名而已
#define Y_API YAPI

// 在unix和win32平台兼容多字节字符
// YTEXT宏表示让编译器把YTEXT里的字符当成多字节字符处理，也就是一个字符可能需要2-4个字节去存储
// 多字节字符可以存储汉字
#ifdef Y_ENV_WIN32
#define YTEXT(quote)		TEXT(quote)
#elif Y_ENV_MINGW
#define YTEXT(quote)		L##quote
#elif Y_ENV_UNIX
#define YTEXT(quote)        quote
#endif

#ifdef __cplusplus
extern "C" {
#endif

    /*
     * 描述：
     * 睡眠指定的毫秒数
     * 
     * 参数：
     * @mseconds：要睡眠的毫秒数
     */
    YAPI void Ysleep(int mseconds);

#ifdef UNIX

    YAPI int fd_read_until(int fd, char *buf, size_t len, char terminator);

    /*
     * 描述：
     * 向一个文件描述符里写入数据，直到写完len大小或者失败才返回
     * 
     * 参数：
     * @fd：要写入的文件描述符
     * @data：要写入的数据地址
     * @len：要写入的数据大小
     * 
     * 返回值：
     * 写入成功返回0，写入失败返回1
     */
    YAPI int fd_write(int fd, void *data, size_t len);

#endif

    YAPI int getflag(int value, int flag);

    YAPI int setflag(int value, int flag);

#ifdef __cplusplus
}
#endif

#endif


