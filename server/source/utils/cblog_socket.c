#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#elif (defined(ENV_UNIX))
#endif

#include <libY.h>

#include "cblog_errors.h"
#include "cblog_socket.h"

int cblog_socket_init()
{
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
	WORD version = MAKEWORD(1, 1);
	WSADATA wsaData;
	int rc = WSAStartup(version, &wsaData);
	if(rc != 0)
	{
		YLOGE("WASStartup failed, %s", strerror(errno));
		return CBLOG_ERR_SYSERR;
	}
#endif

	return CBLOG_ERR_OK;
}

int cblog_socket_get_avaliable_size(cblog_socket sock)
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

int cblog_socket_set_nonblock(cblog_socket sock)
{
	u_long on = 1;
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
	if(ioctlsocket(sock, FIONBIO, &on) != 0)
#elif (defined(ENV_UNIX))
	int val = fcntl(sock, F_GETFL, 0);
	fcntl(sock, F_SETFL, val | O_NONBLOCK);
#endif
	{
		YLOGE("set socket nonblock failed, %d", cblog_socket_error());
		return CBLOG_ERR_SYSERR;
	}

	return CBLOG_ERR_OK;
}

void cblog_socket_close(cblog_socket sock)
{
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
	closesocket(sock);
#elif (defined(ENV_UNIX))
	close(sock);
#endif
}

int cblog_socket_error()
{
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
	return WSAGetLastError();
#elif (defined(ENV_UNIX))
	return errno;
#endif
}