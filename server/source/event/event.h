#ifndef __EVENT_POLL_H__
#define __EVENT_POLL_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "steak_socket.h"
#include "cblog_types.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct event_module_s event_module;
	typedef struct event_module_options_s event_module_options;

	typedef enum eventpoll_type_enum eventpoll_type_enum;
	typedef struct eventpoll_actions_s eventpoll_actions;
	typedef struct steak_event_s steak_event;
	typedef enum steak_event_status_enum steak_event_status_enum;
	typedef enum steak_event_type_enum steak_event_type_enum;

	enum steak_event_status_enum
	{
		/// <summary>
		/// 需要轮询的事件
		/// </summary>
		STEAK_EVENT_STATUS_POLL,
	};

	enum steak_event_type_enum
	{
		/// <summary>
		/// svchost事件
		/// </summary>
		STEAK_EVENT_TYPE_SVCHOST,

		/// <summary>
		/// HTTP客户端链接事件
		/// </summary>
		STEAK_EVENT_TYPE_CONNECTION
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
		/// 监控的文件描述符列表
		/// </summary>
		Ylist *event_list;

		/// <summary>
		/// 有事件触发的事件列表
		/// </summary>
		Ylist *process_event_list;

		/// <summary>
		/// 指定监控事件的超时时间
		/// 单位是毫秒
		/// </summary>
		int timeout_ms;
	};

	struct steak_event_s
	{
		/// <summary>
		/// 该事件所监控的socket
		/// </summary>
		steak_socket sock;

		/// <summary>
		/// 当事件可读的时候触发
		/// </summary>
		int(*on_read)(event_module *evm, steak_event *evt);

		/// <summary>
		/// 当事件可写的时候触发
		/// </summary>
		int(*on_write)(event_module *evm, steak_event *evt);

		/// <summary>
		/// 当事件出现异常情况的时候触发
		/// </summary>
		int(*on_except)(event_module *evm, steak_event *evt);

		/// <summary>
		/// 该事件是否可读
		/// </summary>
		int read;

		/// <summary>
		/// 该事件是否可写
		/// </summary>
		int write;

		int sigread;
		int sigwrite;

		/// <summary>
		/// 该事件所对应的上下文数据
		/// </summary>
		void *context;

		/// <summary>
		/// 该事件的状态
		/// </summary>
		steak_event_status_enum status;

		/// <summary>
		/// 事件类型
		/// </summary>
		steak_event_type_enum type;

		/// <summary>
		/// 该事件的超时时间
		/// 当该事件在该超时时间内没有网络活动，那么就判断为超时事件并删除该事件
		/// </summary>
		int timeout_ms;
	};

	struct eventpoll_actions_s
	{
		eventpoll_type_enum type;
		int(*initialize)(event_module *evm);
		void(*release)(event_module *evm);
		int(*add_event)(event_module *evm, steak_event *evt);
		int(*delete_event)(event_module *evm, steak_event *evt);
		int(*modify_event)(event_module *evm, steak_event *evt, int read, int write);
		int(*modify_write)(event_module *evm, steak_event *evt, int write);
		int(*poll_event)(event_module *evm, steak_event **events, int nevent);
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
	int event_add(event_module *evm, steak_event *evt);

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
	int event_remove(event_module *evm, steak_event *evt);

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
	int event_modify(event_module *evm, steak_event *evt, int read, int write);

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
	int event_poll(event_module *evm, steak_event **events, int nevent);

	/*
	 * 描述：
	 * 处理收到信号了的事件
	 * 处理完之后，需要清空process_event_list
	 *
	 * 参数：
	 * @evm：event_module对象
	 * @events：要处理的事件列表，如果事件列表里的事件没有触发，那么有可能会阻塞
	 * @nevent：事件列表里事件的数量
	 */
	int event_process(event_module *evm, steak_event **events, int nevent);

	/*
	 * 描述：
	 * 创建一个事件的实例
	 *
	 * 参数：
	 * @evm：event_module对象
	 * @sock：客户端socket
	 * @svc：该链接所属的service
	 *
	 * 返回值：
	 * event实例
	 */
	steak_event *new_connection_event(event_module *evm, steak_socket sock, svchost *svc);
	void free_connection_event(event_module *evm, steak_event *evt);
	
	steak_event *new_svchost_event(event_module *evm, svchost *svc);

#ifdef __cplusplus
}
#endif

#endif