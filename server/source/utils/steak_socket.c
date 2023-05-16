#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#elif (defined(ENV_UNIX))
#include <sys/ioctl.h>
#endif

#include <libY.h>

#include "errors.h"
#include "steak_socket.h"

int steak_socket_init()
{
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
	WORD version = MAKEWORD(1, 1);
	WSADATA wsaData;
	int rc = WSAStartup(version, &wsaData);
	if(rc != 0)
	{
		YLOGE("WASStartup failed, %s", strerror(errno));
		return STEAK_ERR_SYSERR;
	}
#endif

	return STEAK_ERR_OK;
}

int steak_socket_get_avaliable_size(steak_socket sock)
{
	u_long size = 0;
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
	if(ioctlsocket(sock, FIONREAD, &size) != 0)
#elif (defined(ENV_UNIX))
	if(ioctl(sock, FIONREAD, &size) != 0)
#endif
	{
		YLOGE("get socket avalible size failed, %s", strerror(errno));
		return -1;
	}

	return size;
}

void steak_socket_close(steak_socket sock)
{
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
	closesocket(sock);
#elif (defined(ENV_UNIX))
	close(sock);
#endif
}