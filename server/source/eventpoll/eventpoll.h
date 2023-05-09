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

	typedef enum eventpoll_type_enum eventpoll_type_enum;
	typedef struct eventpoll_s eventpoll;
	typedef struct eventpoll_options_s eventpoll_options;
	typedef struct eventpoll_actions_s eventpoll_actions;
	typedef struct eventpoll_event_s eventpoll_event;

	enum eventpoll_type_enum
	{
		EVENT_POLL_TYPE_SELECT,
		EVENT_POLL_TYPE_POLL,
		EVENT_POLL_TYPE_EPOLL,
		EVENT_POLL_TYPE_IOCP,
		EVENT_POLL_TYPE_KPOLL
	};

	struct eventpoll_options_s
	{
		eventpoll_type_enum type;
	};

	struct eventpoll_s
	{
		eventpoll_options *options;
		eventpoll_actions *actions;
		void *actions_data;

		//// 文件描述符缓冲池
		//Ypool *FileDescriptorPool;

		// 监控的文件描述符列表
		Ylist *event_list;

		/// <summary>
		/// 指定监控事件的超时时间
		/// </summary>
		int timeout_ms;
	};

	struct eventpoll_event_s
	{
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
		SOCKET sock;
#elif (defined(ENV_UNIX))
		int sock;
#endif

		int(*on_read)(eventpoll *evpoll, eventpoll_event *evt);
		int(*on_write)(eventpoll *evpoll, eventpoll_event *evt);
		int(*on_except)(eventpoll *evpoll, eventpoll_event *evt);

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
		int(*initialize)(eventpoll *evpoll);
		void(*release)(eventpoll *evpoll);
		int(*add_event)(eventpoll *evpoll, eventpoll_event *evt);
		int(*delete_event)(eventpoll *evpoll, eventpoll_event *evt);
		int(*poll_event)(eventpoll *evpoll);
	};

	eventpoll *new_eventpoll(eventpoll_options *options);
	void free_eventpoll(eventpoll *evpoll);
	int eventpoll_add_event(eventpoll *evpoll, eventpoll_event *evt);
	int eventpoll_delete_event(eventpoll *evpoll, eventpoll_event *evt);

	/*
	 * 描述：
	 * 对所有事件进行一次轮询动作
	 *
	 * 参数：
	 * @evpoll：event_poll对象
	 */
	int eventpoll_poll(eventpoll *evpoll);

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
	eventpoll_event *new_event(eventpoll *evpoll);

	/*
	 * 描述：
	 * 释放一个事件的实例
	 * 当事件不需要监控的时候调用
	 *
	 * 参数：
	 * @evpoll：event_poll对象
	 * @evt：要释放的event对象
	 */
	void free_event(eventpoll *evpoll, eventpoll_event *evt);

#ifdef __cplusplus
}
#endif

#endif