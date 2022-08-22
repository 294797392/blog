/***********************************************************************************
 * @ file    : zeus.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.28 00:54
 * @ brief   : 定义公共类型
 ************************************************************************************/

#ifndef __ZEUS_EVENT_H__
#define __ZEUS_EVENT_H__

#include "zeus_os.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef enum 
	{
		ZEUS_EVENT_WRITE,
		ZEUS_EVENT_READ,
		ZEUS_EVENT_LISTEN
	}zeus_event_type;

	typedef struct zeus_event_s zeus_event;

	typedef int(*zeus_event_handler)(zeus_event *evt);

	zeus_event *new_zeus_event(zfd fd, zeus_event_handler handler);
	void free_zeus_event(zeus_event *evt);

#ifdef __cplusplus
}
#endif

#endif
