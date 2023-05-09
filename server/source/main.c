#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#include <process.h>
#elif (defined(ENV_UNIX))
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#endif

#include <libY.h>

#include "errors.h"
#include "event.h"
#include "svchost.h"

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "libY.lib")
#endif

extern int accept_client_event(event_module *evpoll, steak_event *evt);

int main(int argc, char **argv)
{
	int code = YERR_SUCCESS;

	Y_log_init(NULL);

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
	WORD version = MAKEWORD(1, 1);
	WSADATA wsaData;
	int rc = WSAStartup(version, &wsaData);
#endif

	event_module_options evtpoll_options =
	{
		.type = EVENT_POLL_TYPE_SELECT
	};
	event_module *evtpoll = new_eventpoll(&evtpoll_options);

	svchost_options svc_options =
	{
		.bindaddr = "0.0.0.0",
		.port = 1018,
		.root = "~/",
	};
	svchost *svc = new_svchost(&svc_options);
	steak_event *evt = new_event(evtpoll);
	evt->sock = svc->sock;
	evt->readable = 1;
	evt->on_read = accept_client_event;
	event_add(evtpoll, evt);

	YLOGI("start svchost success");

	while(1)
	{
		code = event_poll(evtpoll);
		if(code != STEAK_ERR_OK)
		{
			YLOGE("poll event failed, %d", code);
			continue;
		}
	}

	return 0;
}
