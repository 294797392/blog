#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>

#include <libY.h>

#include "cblog_app.h"
#include "cblog_errors.h"
#include "cblog_types.h"
#include "cblog_socket.h"
#include "cblog_event_module.h"
#include "cblog_factory.h"

int accept_client_event(event_module *evm, cblog_event *evt)
{
	svchost *svc = (svchost *)evt->context;

	struct sockaddr addr;
	size_t addr_len = sizeof(struct sockaddr);
	cblog_socket sock;
	if((sock = accept(evt->sock, &addr, &addr_len)) == -1)
	{
		YLOGE("accept client failed, %s", strerror(errno));
		return CBLOG_ERR_FAILED;
	}

	YLOGI("client connected, new session");

	// 设置套接字为非阻塞
	cblog_socket_set_nonblock(sock);

	// 客户端连接成功，把客户端socket加到待监控的事件列表里
	cblog_event *event = new_cblog_connection_event(sock, svc);
	event_add(evm, event);

	return CBLOG_ERR_OK;
}
