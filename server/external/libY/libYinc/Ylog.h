/***********************************************************************************
 * @ file    : Ylog.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2021.12.06 13:15
 * @ brief   : 一个简易的日志记录器，支持多线程
 * @ remark  : wprintf and printf cannot use in same env
 ************************************************************************************/

#ifndef __YLOG_H__
#define __YLOG_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Y.h"

#define MAX_MSG_SIZE1        4096				// 字符个数，不管是宽字符还是多字节字符

#define YLOGD2(logger, format, ...)		Y_log_write(logger, YLOG_LEVEL_DEBUG, __LINE__, format, __VA_ARGS__)
#define YLOGI2(logger, format, ...)		Y_log_write(logger, YLOG_LEVEL_INFO, __LINE__, format, __VA_ARGS__)
#define YLOGW2(logger, format, ...)		Y_log_write(logger, YLOG_LEVEL_WARN, __LINE__, format, __VA_ARGS__)
#define YLOGE2(logger, format, ...)		Y_log_write(logger, YLOG_LEVEL_ERROR, __LINE__, format, __VA_ARGS__)

#define YLOGD(format, ...)		Y_log_write(NULL, YLOG_LEVEL_DEBUG, __LINE__, format, ##__VA_ARGS__)
#define YLOGI(format, ...)		Y_log_write(NULL, YLOG_LEVEL_INFO, __LINE__, format, ##__VA_ARGS__)
#define YLOGW(format, ...)		Y_log_write(NULL, YLOG_LEVEL_WARN, __LINE__, format, ##__VA_ARGS__)
#define YLOGE(format, ...)		Y_log_write(NULL, YLOG_LEVEL_ERROR, __LINE__, format, ##__VA_ARGS__)

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
	char msg[MAX_MSG_SIZE1];
}Ymsg;

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct Ylogger_s Ylogger;
	typedef struct Ylogger_options_s
	{
		int level;
		YCHAR path[256];
		int max_size_bytes;
		YCHAR format[1024];
	}Ylogger_options;

	/*
	 * 描述：
	 * Ylog全局初始化函数，该函数在整个应用程序里只调用一次
	 *
	 * 返回值：
	 * YERRNO
	 */
	YAPI int Y_log_init(const YCHAR *config);

	YAPI Ylogger *Y_log_get_logger(const YCHAR *name);

	YAPI void Y_log_write(Ylogger *logger, Ylog_level level, int line, YCHAR *msg, ...);

#ifdef __cplusplus
}
#endif

#endif

