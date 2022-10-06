/***********************************************************************************
 * @ file    : platform.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.10.06 17:43
 * @ brief   : 封装不同平台下的统一接口
 ************************************************************************************/

#ifndef __LISTENING_H__
#define __LISTENING_H__

#include <libY.h>

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(Y_WIN32)) || (defined(Y_MINGW))
#define syserror()      GetLastError()
#endif

#ifdef __cplusplus
}
#endif

#endif

