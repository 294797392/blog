/***********************************************************************************
 * @ file    : cblog_http_event.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.23 13:02
 * @ brief   : ����HTTP���������¼�����
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
		/// �յ�HTTP���� - event
		/// </summary>
		CBLOG_HTTP_EVENT_BEGIN_REUQEST = 0,

		/// <summary>
		/// �յ�HTTP���� - ����������� - event
		/// </summary>
		CBLOG_HTTP_EVENT_END_REQUEST = 1,

		/// <summary>
		/// ����������� - event - ������Ӧ
		/// </summary>
		CBLOG_HTTP_EVENT_BEGIN_RESPONSE = 2,

		/// <summary>
		/// ������Ӧ���� - event - ������Ӧ
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
	/// ע��http�¼�
	/// </summary>
	/// <param name="app"></param>
	/// <param name="evt"></param>
	/// <param name="handler"></param>
	/// <returns></returns>
	void cblog_http_add_event(cblog_http_event_list *evlist, cblog_http_event_enum evtype, cblog_http_event_handler handler);
	void cblog_http_remove_event(cblog_http_event_list *evlist, cblog_http_event_enum evtype, cblog_http_event_handler handler);

	/// <summary>
	/// ����ĳ��HTTP�¼�
	/// ֻҪע��������¼���http_module�����ᴥ����Ӧ���¼�
	/// </summary>
	/// <param name="evlist"></param>
	/// <param name="evtype"></param>
	/// <param name="conn"></param>
	void cblog_http_raise_event(cblog_http_event_list *evlist, cblog_http_event_enum evtype, cblog_connection *conn);

#ifdef __cplusplus
}
#endif

#endif