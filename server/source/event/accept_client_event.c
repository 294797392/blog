#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>

#include <libY.h>

#include "steak_socket.h"
#include "factory.h"
#include "errors.h"
#include "event.h"
#include "app.h"
#include "connection.h"

extern int read_request_event(event_module *evm, steak_event *evt);
extern int write_response_event(event_module *evm, steak_event *evt);

int accept_client_event(event_module *evm, steak_event *evt)
{
	struct sockaddr addr;
	size_t addr_len = sizeof(struct sockaddr);
	steak_socket sock;
	if((sock = accept(evt->sock, &addr, &addr_len)) == -1)
	{
		YLOGE("accept client failed, %s", strerror(errno));
		return STEAK_ERR_FAILED;
	}

	YLOGI("client connected, new session");

	// 客户端连接成功，把客户端socket加到待监控的事件列表里
	steak_connection *conn = new_connection(sock);
	steak_event *event = new_connection_event(evm, read_request_event, write_response_event, conn);
	event_add(evm, event);

	return STEAK_ERR_OK;
}
