/***********************************************************************************
 * @ file    : zeus_log.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.23 9:07
 * @ brief   : 日志记录器
 ************************************************************************************/

#ifndef __ZEUS_LOG_H__
#define __ZEUS_LOG_H__

#include "zeus_string.h"

#ifdef ZEUS_WIN32
#endif

void zlogd(zchar *text, ...);
void zlogi(zchar *text, ...);
void zlogw(zchar *text, ...);
void zloge(zchar *text, ...);

#endif