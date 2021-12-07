#ifndef __YSTRING_H__
#define __YSTRING_H__

#include "Yfirstinclude.h"

#include "Ybase.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum Ystr_split_opts_e
    {
        SSO_None = 0,
        SSO_RemoveEmptyEntries = 1
    }Ystr_split_opts;

    YAPI void Ystr_trim_right(char *str, char c);

    /*
     * 描述：
     * 生成一个随机数
     * 
     * 参数：
     * @buffer：存储随机数的缓冲区
     * @length：缓冲区大小
     */
    YAPI void Ystr_rand(char *buffer, size_t length);

    /*
     * 描述：
     * 把str按照separator进行分割
     * 
     * 参数：
     * @str：要分割的字符串
     * @separator：分隔符
     * @opt：分割选项
     * @tokens：存储分割后的字符串的数组
     * @total_tokens：字符串数组的长度
     * 
     * 返回：
     * 分割出的字符串的个数
     */
    YAPI int Ystr_split(const char *str, const char separator, Ystr_split_opts opt, char **tokens, size_t total_tokens);

    /*
     * 描述：
     * 打印分割后的字符串（调试用）
     * 
     * 参数：
     * @num：字符串数组的长度
     * @tokens：分割后的字符串数据
     */
    YAPI void Ystr_split_print(int num, char **tokens);

    /*
     * 描述：
     * 释放分割后的字符串内存
     * 
     * 参数：
     * @num_tokens：字符串数组的长度
     * @tokens：要释放的字符串数组
     */
    YAPI void Ystr_split_free(size_t num_tokens, char **tokens);

#ifdef __cplusplus
}
#endif

#endif