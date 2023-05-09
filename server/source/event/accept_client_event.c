#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>

#include <libY.h>

#include "errors.h"
#include "event.h"
#include "app.h"

extern int read_request_event(event_module *evm, steak_event *evt);
extern int write_response_event(event_module *evm, steak_event *evt);

int accept_client_event(event_module *evm, steak_event *evt)
{
	struct sockaddr addr;
	size_t addr_len = sizeof(struct sockaddr);
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
	SOCKET sock;
#elif (defined(ENV_UNIX))
	int sock;
#endif
	if((sock = accept(evt->sock, &addr, &addr_len)) == -1)
	{
		YLOGE("accept client failed, %s", strerror(errno));
		return STEAK_ERR_FAILED;
	}

	YLOGI("client connected, new session");

	steak_session *session = new_session();

	steak_event *event = new_event(evm);
	event->writeable = 1;
	event->readable = 1;
	event->sock = sock;
	event->on_read = read_request_event;
	event->on_write = write_response_event;
	event->context = session;
	event_add(evm, event);

	return STEAK_ERR_OK;
}
