#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>

#include <libY.h>

#include "errors.h"
#include "event.h"
#include "app.h"

/// <summary>
/// 2023-5-18 20:17
/// 可以发送响应给HTTP客户端了
/// </summary>
/// <param name="conn"></param>
static void send_hello_cblog(steak_connection *conn)
{
	char *str = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nhello cblog!";
	send(conn->sock, str, strlen(str), 0);
}

int write_response_event(event_module *evm, cblog_event *evt)
{
	steak_connection *conn = (steak_connection *)evt->context;

	send_hello_cblog(conn);

	steak_socket_close(evt->sock);
	event_remove(evm, evt);
	free_connection_event(evm, evt);

	return STEAK_ERR_OK;
}
