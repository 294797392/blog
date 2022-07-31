/***********************************************************************************
 * @ file    : zeus_poll.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.26 21:00
 * @ brief   : 封装socket网络模型（select，epoll，IOCP..）
 ************************************************************************************/

#ifndef __ZEUS_EVENT_DRIVER_H__
#define __ZEUS_EVENT_DRIVER_H__

#include "zeus_os.h"
#include "zeus_config.h"
#include "zeus_event.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct zeus_event_driver_s zeus_event_driver;
	typedef struct zeus_event_driver_operation_s zeus_event_driver_operation;

	// 定义所支持的socket网络模型
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
		void *context;
		int(*initialize)(zeus_event_driver *drv);
		int(*add_event)(zeus_event_driver *drv, zeus_event *evt);
		void(*remove_event)(zeus_event_driver *drv, zeus_event *evt);
	};

	struct zeus_event_driver_s
	{
		zeus_event_driver_operation *operations;
		void *context;
	};

	/*
	 * 描述：
	 * 新建一个事件监控器
	 *
	 * 参数：
	 * @config：配置文件
	 *
	 * 返回值：
	 * 新建的zeus_event_driver对象
	 */
	zeus_event_driver *new_event_driver(zeus_config *config);

	/*
	 * 描述：
	 * 释放事件监控器占用的资源
	 *
	 * 参数：
	 * @drv：要释放的事件监控器
	 */
	void free_event_driver(zeus_event_driver *drv);

	/*
	 * 描述：
	 * 新增一个要监控的事件
	 * 当事件触发的时候，调用evt->handler
	 *
	 * 参数：
	 * @driver：事件监控器的句柄
	 * @evt：要监控的事件
	 *
	 * 返回值：参考ZEUS_ERR
	 */
	int zeus_event_driver_add_event(zeus_event_driver *driver, zeus_event *evt);

	/*
	 * 描述：
	 * 从事件监控器里删除一个不需要再监控的事件
	 *
	 * 参数：
	 * @driver：事件监控器的句柄
	 * @evt：要删除的事件
	 *
	 * 返回值：参考ZEUS_ERR
	 */
	int zeus_event_driver_remove_event(zeus_event_driver *driver, zeus_event *evt);

#ifdef __cplusplus
}
#endif

#endif

