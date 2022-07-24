/***********************************************************************************
 * @ file    : zeus_worker.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.23 9:20
 * @ brief   : 处理请求的工作者线程
 ************************************************************************************/


#ifndef __ZEUS_WORKER_H__
#define __ZEUS_WORKER_H__

#ifdef ZEUS_WIN32
#include <Windows.h>
#endif

#include "zeus.h"

typedef enum
{
    ZW_STATE_IDLE,
    ZW_STATE_RUNNING,
}ZEUS_WORKER_STATE;

typedef struct zeus_worker_s
{
    zeus *z;

    // 工作者的序号
    int index;

    // 工作者当前的状态
    ZEUS_WORKER_STATE state;

    HANDLE event;

    SOCKET client;

}zeus_worker;

void zeus_worker_main(void *userdata);

zeus_worker *select_idle_worker(zeus *z);

#endif

