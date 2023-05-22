/***********************************************************************************
 * @ file    : cblog_factory.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.21 21:02
 * @ brief   : 提供创建/释放对象实例的函数
 ************************************************************************************/

#ifndef __CBLOG_FACTORY_H__
#define __CBLOG_FACTORY_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "cblog_socket.h"
#include "cblog_types.h"
#include "cblog_event_module.h"

#ifdef __cplusplus
extern "C" {
#endif

	cblog_request *new_cblog_request();
	void free_cblog_request(cblog_request *request);

	cblog_response *new_cblog_response();
	void free_cblog_response(cblog_response *response);

	cblog_pending_response *new_cblog_pending_response(cblog_response *response);
	cblog_pending_response *new_cblog_pending_response2(char *buf, int buflen);
	void free_cblog_pending_response(cblog_pending_response *response);

	cblog_connection *new_cblog_connection(cblog_socket sock, svchost *svc);
	void free_cblog_connection(cblog_connection *conn);

	/*
	 * 描述：
	 * 创建一个事件的实例
	 *
	 * 参数：
	 * @sock：客户端socket
	 * @svc：该链接所属的service
	 *
	 * 返回值：
	 * event实例
	 */
	cblog_event *new_cblog_connection_event(cblog_socket sock, svchost *svc);
	void free_cblog_connection_event(cblog_event *evt);

	cblog_event *new_cblog_svchost_event(svchost *svc);
	void free_cblog_svchost_event(cblog_event *evt);

	cblog_http_header *new_cblog_http_header();
	void free_cblog_http_header(cblog_http_header *header);

#ifdef __cplusplus
}
#endif

#endif