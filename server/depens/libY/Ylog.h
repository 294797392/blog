/***********************************************************************************
 * @ file    : Ylog.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2021.12.06 13:15
 * @ brief   : 一个简易的日志记录器，支持多线程
 ************************************************************************************/

#ifndef __YLOG_H__
#define __YLOG_H__

#include "Yfirstinclude.h"

#include "Ybase.h"
#include "Ylogbase.h"

#define YLOGD(...) Ylog_write(NULL, YLOG_LEVEL_DEBUG, __LINE__, __VA_ARGS__)
#define YLOGI(...) Ylog_write(NULL, YLOG_LEVEL_INFO, __LINE__, __VA_ARGS__)
#define YLOGE(...) Ylog_write(NULL, YLOG_LEVEL_ERROR, __LINE__, __VA_ARGS__)
#define YLOGW(...) Ylog_write(NULL, YLOG_LEVEL_WARN, __LINE__, __VA_ARGS__)

#define YLOGCD(cate, ...) Ylog_write(cate, YLOG_LEVEL_DEBUG, __LINE__, __VA_ARGS__)
#define YLOGCI(cate, ...) Ylog_write(cate, YLOG_LEVEL_INFO, __LINE__, __VA_ARGS__)
#define YLOGCE(cate, ...) Ylog_write(cate, YLOG_LEVEL_ERROR, __LINE__, __VA_ARGS__)
#define YLOGCW(cate, ...) Ylog_write(cate, YLOG_LEVEL_WARN, __LINE__, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * 描述：
	 * Ylog全局初始化函数，该函数在整个应用程序里只调用一次
	 *
	 * 返回值：
	 * tcpsvc对象
	 */
	YAPI int Ylog_global_init();

	YAPI void Ylog_write(const wchar_t *category, Ylog_level level, int line, const wchar_t *msg, ...);

#ifdef __cplusplus
}
#endif

#endif