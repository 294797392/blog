/***********************************************************************************
 * @ file    : cblog_http_event.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.23 13:02
 * @ brief   : 管理HTTP生命周期事件队列
 ************************************************************************************/

#ifndef __CBLOG_HTTP_EVENT_H__
#define __CBLOG_HTTP_EVENT_H__

#include "cblog_default.h"
#include "cblog_types.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef int(*cblog_http_event_handler)(cblog_http_context *http_context);
	typedef enum cblog_http_event_enum cblog_http_event_enum;
	typedef struct cblog_http_event_s cblog_http_event;
	typedef struct cblog_http_event_chain_s cblog_http_event_chain;
	typedef struct cblog_http_event_list_s cblog_http_event_list;

	enum cblog_http_event_enum
	{
		/// <summary>
		/// 收到HTTP请求 - event
		/// </summary>
		CBLOG_HTTP_EVENT_BEGIN_REUQEST = 0,

		/// <summary>
		/// 收到HTTP请求 - 接收请求结束 - event
		/// </summary>
		CBLOG_HTTP_EVENT_END_REQUEST = 1,

		/// <summary>
		/// 接收请求结束 - event - 生成响应
		/// </summary>
		CBLOG_HTTP_EVENT_BEGIN_RESPONSE = 2,

		/// <summary>
		/// 生成响应结束 - event - 发送响应
		/// </summary>
		CBLOG_HTTP_EVENT_END_RESPONSE = 3
	};

	struct cblog_http_event_s
	{
		cblog_http_event *prev;
		cblog_http_event *next;
		cblog_http_event_handler handler;
	};

	struct cblog_http_event_chain_s
	{
		cblog_http_event *first;
		cblog_http_event *last;
		int count;
	};

	struct cblog_http_event_list_s
	{
		cblog_http_event_chain chain_list[CBLOG_DEFAULT_HTTP_EVENT_COUNT];
	};

	/// <summary>
	/// 注册http事件
	/// </summary>
	/// <param name="app"></param>
	/// <param name="evt"></param>
	/// <param name="handler"></param>
	/// <returns></returns>
	void cblog_http_add_event(cblog_http_event_list *evlist, cblog_http_event_enum evtype, cblog_http_event_handler handler);
	void cblog_http_remove_event(cblog_http_event_list *evlist, cblog_http_event_enum evtype, cblog_http_event_handler handler);

	/// <summary>
	/// 触发某个HTTP事件
	/// 只要注册了这个事件的http_module，都会触发对应的事件
	/// </summary>
	/// <param name="evlist"></param>
	/// <param name="evtype"></param>
	/// <param name="conn"></param>
	void cblog_http_raise_event(cblog_http_event_list *evlist, cblog_http_event_enum evtype, cblog_connection *conn);

#ifdef __cplusplus
}
#endif

#endif