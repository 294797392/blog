/***********************************************************************************
 * @ file    : FDEvents.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.12.01 21:39
 * @ brief   : 定义所有的文件描述符事件处理器
 ************************************************************************************/
#ifndef __FDEVENTS_H__
#define __FDEVENTS_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "FDEventPoll.h"

extern int AcceptClientEvent(FDEventPoll *monitor, FileDescriptor *fd);

#endif