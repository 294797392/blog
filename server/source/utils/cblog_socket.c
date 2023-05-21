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

cblog_socket_buffer *new_cblog_sockbuf(cblog_socket sock, int size)
{
	cblog_socket_buffer *buffer = (cblog_socket_buffer *)calloc(1, sizeof(cblog_socket_buffer));
	buffer->sock = sock;
	buffer->ptr = (char *)calloc(1, size);
	buffer->size = size;
	buffer->left = size;
	buffer->offset = 0;
	return buffer;
}

void free_cblog_sockbuf(cblog_socket_buffer *buffer)
{
	free(buffer->ptr);
	free(buffer);
}

int cblog_sockbuf_recv(cblog_socket_buffer *buffer)
{
	// 获取当前socket缓冲区里的数据大小
	int avaliable = cblog_socket_get_avaliable_size(buffer->sock);
	if(avaliable == -1)
	{
		// 返回0，外部模块会关闭连接
		return 0;
	}

	// 剩余缓冲区大小比要接收的数据大小小，扩容缓冲区
	if(buffer->left < avaliable)
	{
		int size1 = buffer->size * 2;
		int size2 = avaliable;
		int newsize = size1 > size2 ? size1 : size2;

		buffer->ptr = realloc(buffer->ptr, newsize);
		buffer->size = newsize;
		buffer->left = buffer->size - buffer->offset;
	}

	// 接收数据
	int rc = recv(buffer->sock, buffer->ptr + buffer->offset, avaliable, 0);
	if(rc > 0)
	{
		buffer->offset += rc;
		buffer->left -= rc;
	}
	return rc;
}

void cblog_sockbuf_reset(cblog_socket_buffer *buffer)
{
	buffer->left = buffer->size;
	buffer->offset = 0;
}





int cblog_socket_init()
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
		return STEAK_ERR_SYSERR;
	}

	return STEAK_ERR_OK;
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