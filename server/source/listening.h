/***********************************************************************************
 * @ file    : listening.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.09.30 06:37
 * @ brief   : 监听
 ************************************************************************************/

#ifndef __LISTENING_H__
#define __LISTENING_H__

#include <libY.h>

#include "errors.h"
#include "blog.h"

#ifdef __cplusplus
extern "C" {
#endif

    Ylist *create_listens(cJSON *config);

    int open_listens(Ylist *listens);

#ifdef __cplusplus
}
#endif

#endif

