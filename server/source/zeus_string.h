/***********************************************************************************
 * @ file    : zeus_string.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.23 8:21
 * @ brief   : 封装字符串相关的操作，为了支持直接输出中文
 ************************************************************************************/

#ifndef __ZEUS_BASE_H__
#define __ZEUS_BASE_H__


#ifdef ZEUS_WIN32
#define ZTEXT(string) L##string
#elif ZEUS_LINUX
#define ZTEXT(string)
#endif

#ifdef __cplusplus
extern "C" {
#endif

    typedef wchar_t zchar;

    void zprintf(zchar *format, ...);

    /*
     * 描述：
     * 计算c在str中的索引位置
     * 
     * 参数：
     * @str：源字符串
     * @c：要计算位置的字符
     * 
     * 返回值：
     * c在str字符串中的索引位置，如果没找到则返回-1
     * str到c之间的字符数量，包含start字符和c字符
     */
    int zeus_charat(const char *str, char c);

    int zeus_charat2(const char *str, char c, char *value);

#ifdef __cplusplus
}
#endif

#endif