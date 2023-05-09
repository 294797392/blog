#ifndef __EVENT_POLL_H__
#define __EVENT_POLL_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#elif (defined(ENV_UNIX))
#endif

#include <libY.h>

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct event_module_s event_module;
	typedef struct event_module_options_s event_module_options;

	typedef enum eventpoll_type_enum eventpoll_type_enum;
	typedef struct eventpoll_actions_s eventpoll_actions;
	typedef struct steak_event_s steak_event;

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
		event_module_options *options;
		eventpoll_actions *actions;
		void *actions_data;

		//// 文件描述符缓冲池
		//Ypool *FileDescriptorPool;

		// 监控的文件描述符列表
		Ylist *event_list;

		/// <summary>
		/// 指定监控事件的超时时间
		/// 单位是毫秒
		/// </summary>
		int timeout_ms;
	};

	struct steak_event_s
	{
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
		SOCKET sock;
#elif (defined(ENV_UNIX))
		int sock;
#endif

		/// <summary>
		/// 当事件可读的时候触发
		/// </summary>
		int(*on_read)(event_module *evpoll, steak_event *evt);

		/// <summary>
		/// 当事件可写的时候触发
		/// </summary>
		int(*on_write)(event_module *evpoll, steak_event *evt);

		/// <summary>
		/// 当事件出现异常情况的时候触发
		/// </summary>
		int(*on_except)(event_module *evpoll, steak_event *evt);

		/// <summary>
		/// 该事件是否可读
		/// </summary>
		int readable;

		/// <summary>
		/// 该事件是否可写
		/// </summary>
		int writeable;

		/// <summary>
		/// 该事件所对应的上下文数据
		/// </summary>
		void *context;
	};

	struct eventpoll_actions_s
	{
		eventpoll_type_enum type;
		int(*initialize)(event_module *evpoll);
		void(*release)(event_module *evpoll);
		int(*add_event)(event_module *evpoll, steak_event *evt);
		int(*delete_event)(event_module *evpoll, steak_event *evt);
		int(*poll_event)(event_module *evpoll);
	};

	event_module *new_eventpoll(event_module_options *options);
	void free_eventpoll(event_module *evm);

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
	int event_delete(event_module *evm, steak_event *evt);

	/*
	 * 描述：
	 * 对所有事件进行一次轮询动作
	 *
	 * 参数：
	 * @evpoll：event_poll对象
	 */
	int event_poll(event_module *evm);

	/*
	 * 描述：
	 * 创建一个事件的实例
	 *
	 * 参数：
	 * @evpoll：event_poll对象
	 * 
	 * 返回值：
	 * event实例
	 */
	steak_event *new_event(event_module *evm);

	/*
	 * 描述：
	 * 释放一个事件的实例
	 * 当事件不需要监控的时候调用
	 *
	 * 参数：
	 * @evpoll：event_poll对象
	 * @evt：要释放的event对象
	 */
	void free_event(event_module *evm, steak_event *evt);

#ifdef __cplusplus
}
#endif

#endif