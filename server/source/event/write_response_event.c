#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>

#include <libY.h>

#include "cblog_errors.h"
#include "cblog_app.h"
#include "cblog_event_module.h"

/// <summary>
/// 2023-5-18 20:17
/// 可以发送响应给HTTP客户端了
/// </summary>
/// <param name="conn"></param>
static void send_hello_cblog(cblog_connection *conn)
{
	char *str = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nhello cblog!";
	send(conn->sock, str, strlen(str), 0);
}

int write_response_event(event_module *evm, cblog_event *evt)
{
	cblog_connection *conn = (cblog_connection *)evt->context;

	send_hello_cblog(conn);

	event_modify(evm, evt, evt->read, 0);

	// 发送完响应，不需要keep-alive
	// 直接关闭连接
	if(!conn->keep_alive)
	{
		Y_queue_enqueue(evm->except_events, evt);
	}

	return STEAK_ERR_OK;
}
