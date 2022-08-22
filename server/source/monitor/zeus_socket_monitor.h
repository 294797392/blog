/***********************************************************************************
 * @ file    : zeus_socket_monitor.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.26 21:00
 * @ brief   : 管理socket文件描述符的模块
 ************************************************************************************/

#ifndef __ZEUS_SOCKET_MONITOR_H__
#define __ZEUS_SOCKET_MONITOR_H__

#include "zeus_os.h"
#include "zeus_config.h"
#include "zeus_event.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct zeus_event_mgr_s zeus_event_mgr;
	typedef struct zeus_event_mgr_actions_s zeus_event_mgr_actions;

	// 定义所支持的socket网络模型
	typedef enum
	{
		ZEUS_EVT_DRV_SELECT,
		ZEUS_EVT_DRV_EPOLL,
		ZEUS_EVT_DRV_IOCP,
		ZEUS_EVT_DRV_KQUEUE
	}ZEUS_EVENT_MGR_TYPE;

	struct zeus_event_mgr_actions_s
	{
		const char *name;
		const char *author;
		ZEUS_EVENT_MGR_TYPE type;
		int(*initialize)(zeus_event_mgr *mgr);
		int(*add_event)(zeus_event_mgr *mgr, zeus_event *evt);
		void(*remove_event)(zeus_event_mgr *mgr, zeus_event *evt);
	};

	struct zeus_event_mgr_s
	{
		zeus_event_mgr_actions *actions;
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
	zeus_event_mgr *new_event_mgr(zeus_config *config);

	/*
	 * 描述：
	 * 释放事件监控器占用的资源
	 *
	 * 参数：
	 * @drv：要释放的事件监控器
	 */
	void free_event_mgr(zeus_event_mgr *drv);

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
	int zeus_event_mgr_add_event(zeus_event_mgr *driver, zeus_event *evt);

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
	int zeus_event_mgr_remove_event(zeus_event_mgr *driver, zeus_event *evt);

#ifdef __cplusplus
}
#endif

#endif

