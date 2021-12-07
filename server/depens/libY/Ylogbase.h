/***********************************************************************************
 * @ file    : Ylogbase.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2021.12.06 13.29
 * @ brief   : Ylog内部使用，调用者用不到
 ************************************************************************************/

#ifndef __YLOG_BASE_H__
#define __YLOG_BASE_H__

#define MAX_MSG_SIZE        4096

// 内部使用，调用者用不到这个枚举
typedef enum Ylog_level_e
{
    YLOG_LEVEL_DEBUG,
    YLOG_LEVEL_INFO,
    YLOG_LEVEL_WARN,
    YLOG_LEVEL_ERROR
}Ylog_level;

typedef struct Ymsg_s
{
    Ylog_level level;
    wchar_t msg[MAX_MSG_SIZE];
}Ymsg;

#endif
