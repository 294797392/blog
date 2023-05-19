#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>

#include <libY.h>

#include "steak_socket.h"
#include "errors.h"
#include "event.h"
#include "app.h"
#include "cblog_types.h"

int accept_client_event(event_module *evm, cblog_event *evt)
{
	svchost *svc = (svchost *)evt->context;

	struct sockaddr addr;
	size_t addr_len = sizeof(struct sockaddr);
	cblog_socket sock;
	if((sock = accept(evt->sock, &addr, &addr_len)) == -1)
	{
		YLOGE("accept client failed, %s", strerror(errno));
		return STEAK_ERR_FAILED;
	}

	YLOGI("client connected, new session");

	// 客户端连接成功，把客户端socket加到待监控的事件列表里
	cblog_event *event = new_connection_event(evm, sock, svc);
	event_add(evm, event);

	return STEAK_ERR_OK;
}
