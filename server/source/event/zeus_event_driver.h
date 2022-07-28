/***********************************************************************************
 * @ file    : zeus_poll.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.26 21:00
 * @ brief   : 封装socket网络模型（select，epoll，IOCP..）
 ************************************************************************************/

#ifndef __ZEUS_EVENT_MON_H__
#define __ZEUS_EVENT_MON_H__

#include "zeus_os.h"
#include "zeus.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct zeus_event_s zeus_event;
	typedef struct zeus_event_driver_operation_s zeus_event_driver_operation;
	typedef struct zeus_event_driver_s zeus_event_driver;

	struct zeus_event_s
	{
		zfd fd;							// 事件所关联的文件描述符
		int(*handler)();				// 当该事件触发后，所执行的动作
	};

	// 定义所支持的socket模型
	typedef enum
	{
		ZEUS_EVT_DRV_SELECT,
		ZEUS_EVT_DRV_EPOLL,
		ZEUS_EVT_DRV_IOCP,
		ZEUS_EVT_DRV_KQUEUE
	}ZEUS_EVENT_DRIVER_TYPE;

	struct zeus_event_driver_operation_s
	{
		const char *name;
		const char *author;
		ZEUS_EVENT_DRIVER_TYPE type;
		void *(*initialize)(zeus_event_driver *drv);
		int(*add_event)(zeus_event_driver *drv, zeus_event *evt);
		void(*remove_event)(zeus_event_driver *drv, zeus_event *evt);
	};

	struct zeus_event_driver_s
	{
		zeus_event_driver_operation *operations;
		void *context;
	};

#ifdef __cplusplus
}
#endif

#endif

