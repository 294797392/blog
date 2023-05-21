/***********************************************************************************
 * @ file    : cblog_event_module.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : ???
 * @ brief   : socket事件监控模块
 ************************************************************************************/

#ifndef __CBLOG_EVENT_MODULE_H__
#define __CBLOG_EVENT_MODULE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "cblog_socket.h"
#include "cblog_types.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct event_module_s event_module;
	typedef struct event_module_options_s event_module_options;
	typedef enum eventpoll_type_enum eventpoll_type_enum;
	typedef struct eventpoll_actions_s eventpoll_actions;
	typedef struct cblog_event_s cblog_event;
	typedef enum cblog_event_type_enum cblog_event_type_enum;
	typedef struct cblog_event_chain_s cblog_event_chain;

	struct cblog_event_chain_s
	{
		cblog_event *first;
		cblog_event *last;
		int count;
	};

	enum cblog_event_type_enum
	{
		/// <summary>
		/// svchost事件
		/// </summary>
		CBLOG_EVENT_TYPE_SVCHOST,

		/// <summary>
		/// HTTP客户端链接事件
		/// </summary>
		CBLOG_EVENT_TYPE_CONNECTION
	};

	enum eventpoll_type_enum
	{
		EVENT_POLL_TYPE_SELECT,
		EVENT_POLL_TYPE_POLL,
		EVENT_POLL_TYPE_EPOLL,
		EVENT_POLL_TYPE_IOCP,
		EVENT_POLL_TYPE_KPOLL
	};

	struct event_module_options_s
	{
		eventpoll_type_enum type;
	};

	struct event_module_s
	{
		event_module_options options;
		eventpoll_actions *actions;
		void *actions_data;

		/// <summary>
		/// 监控的所有事件
		/// </summary>
		cblog_event_chain *events;

		/// <summary>
		/// 出现异常的事件
		/// 比如断开了连接，读写失败等等
		/// </summary>
		Yqueue *except_events;

		/// <summary>
		/// 指定监控事件的超时时间
		/// 单位是毫秒
		/// </summary>
		int timeout_ms;
	};

	struct cblog_event_s
	{
		cblog_event *prev;
		cblog_event *next;

		/// <summary>
		/// 该事件所监控的socket
		/// </summary>
		cblog_socket sock;

		/// <summary>
		/// 当事件可读的时候触发
		/// </summary>
		int(*on_read)(event_module *evm, cblog_event *evt);

		/// <summary>
		/// 当事件可写的时候触发
		/// </summary>
		int(*on_write)(event_module *evm, cblog_event *evt);

		/// <summary>
		/// 当事件出现异常情况的时候触发
		/// </summary>
		int(*on_except)(event_module *evm, cblog_event *evt);

		/// <summary>
		/// 该事件是否可读
		/// </summary>
		int read;

		/// <summary>
		/// 该事件是否可写
		/// </summary>
		int write;

		/// <summary>
		/// 该事件所对应的上下文数据
		/// </summary>
		void *context;

		/// <summary>
		/// 事件类型
		/// </summary>
		cblog_event_type_enum type;

		/// <summary>
		/// 该事件的超时时间
		/// 当该事件在该超时时间内没有网络活动，那么就判断为超时事件并删除该事件
		/// -1为永远不会超时
		/// </summary>
		int timeout_ms;
	};

	struct eventpoll_actions_s
	{
		eventpoll_type_enum type;
		int(*initialize)(event_module *evm);
		void(*release)(event_module *evm);
		int(*add_event)(event_module *evm, cblog_event *evt);
		int(*delete_event)(event_module *evm, cblog_event *evt);
		int(*modify_event)(event_module *evm, cblog_event *evt, int read, int write);
		int(*modify_write)(event_module *evm, cblog_event *evt, int write);
		int(*poll_event)(event_module *evm);
	};

	/*
	 * 描述：
	 * 增加一个要监控的事件
	 *
	 * 参数：
	 * @evm：事件模块
	 * @evt：要监控的事件
	 *
	 * 返回值：
	 * STEAK_ERR
	 */
	int event_add(event_module *evm, cblog_event *evt);

	/*
	 * 描述：
	 * 删除一个要监控的事件
	 *
	 * 参数：
	 * @evm：事件模块
	 * @evt：要删除的事件
	 *
	 * 返回值：
	 * STEAK_ERR
	 */
	int event_remove(event_module *evm, cblog_event *evt);

	/*
	 * 描述：
	 * 更改事件要监控的读或者写
	 *
	 * 参数：
	 * @evm：事件模块
	 * @evt：要删除的事件
	 *
	 * 返回值：
	 * STEAK_ERR
	 */
	int event_modify(event_module *evm, cblog_event *evt, int read, int write);

	/*
	 * 描述：
	 * 对所有事件进行一次轮询动作
	 * 如果有某个事件触发了，需要把触发的事件加到process_event_list里
	 *
	 * 参数：
	 * @evm：event_poll对象
	 * @events：要轮询的事件列表，不在这个列表里的事件不会轮询
	 * @nevent：事件列表里事件的数量
	 */
	int event_run_cycle(event_module *evm);

#ifdef __cplusplus
}
#endif

#endif