#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "steak_socket.h"

int steak_socket_get_avaliable_size(steak_socket sock)
{
	u_long size = 0;
	if(ioctlsocket(sock, FIONREAD, &size) != 0)
	{
		YLOGE("get socket avalible size failed, %s", strerror(errno));
		return -1;
	}

	return size;
}
