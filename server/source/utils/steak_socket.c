#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#elif (defined(ENV_UNIX))
#include <sys/ioctl.h>
#endif

#include <libY.h>

#include "steak_socket.h"

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
