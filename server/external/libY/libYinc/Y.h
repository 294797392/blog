#ifndef __Y_H__
#define __Y_H__

#if (defined(Y_WIN32))
#include <Windows.h>
#elif (defined(Y_UNIX))
#include <unistd.h>
#endif

#define YMAX_PATH                    512

#define YARRAY_LENGTH(_Array) (sizeof(_Array) / sizeof(_Array[0]))

// 定义导出和导入符号
// 注意，如果是链接静态库，那么不需要__declspec(dllimport)
#if (defined(Y_WIN32))
    #ifdef Y_EXPORT
        #define YAPI __declspec(dllexport)
    #else
        #define YAPI __declspec(dllimport)
    #endif
#elif (defined(Y_WIN32))
    #ifdef Y_EXPORT
        #define YAPI __declspec(dllexport)
    #else
        #define YAPI
    #endif
#else
    #define YAPI
#endif

#ifdef UNICODE
typedef wchar_t YCHAR;
#if (defined(Y_WIN32))
#define YTEXT(quote)       TEXT(quote)
#elif (defined(Y_MINGW))
#define YTEXT(quote)        L##quote
#elif (defined(Y_MSYS))
#define YTEXT(quote)        L##quote
#endif
#else
#define YTEXT(quote)
typedef char YCHAR;
#endif

// 表示一个字节
typedef char YBYTE;

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


    YAPI void *Ycalloc(size_t count, size_t size);
    YAPI void Yfree(void *mem);

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


