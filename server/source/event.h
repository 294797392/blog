/***********************************************************************************
 * @ file    : event.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.09.30 08:19
 * @ brief   : 事件管理模块
 ************************************************************************************/

#ifndef __EVENT_H__
#define __EVENT_H__

#include <libY.h>
#include <cJSON.h>

#include "blog.h"

#ifdef __cplusplus
extern "C" {
#endif

    event_poll *create_event_poll(cJSON *config);
    int add_event(event_poll *poll, event *evt);
    void del_event(event_poll *poll, event *evt);
    int poll_events(event_poll *poll);
    int process_events(event_poll *poll);

#ifdef __cplusplus
}
#endif

#endif

