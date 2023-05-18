#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "errors.h"
#include "steak_socket.h"
#include "cblog_types.h"
#include "cblog_sockbuf.h"

cblog_sockbuf *new_cblog_sockbuf(steak_socket sock, int size)
{
	cblog_sockbuf *buffer = (cblog_sockbuf *)calloc(1, sizeof(cblog_sockbuf));
	buffer->sock = sock;
	buffer->ptr = (char *)calloc(1, size);
	buffer->size = size;
	buffer->left = size;
	buffer->offset = 0;
	return buffer;
}

void free_cblog_sockbuf(cblog_sockbuf *buffer)
{
	free(buffer->ptr);
	free(buffer);
}

int cblog_sockbuf_recv(cblog_sockbuf *buffer)
{
	// 获取当前socket缓冲区里的数据大小
	int avaliable = steak_socket_get_avaliable_size(buffer->sock);
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

void cblog_sockbuf_reset(cblog_sockbuf *buffer)
{
	buffer->left = buffer->size;
	buffer->offset = 0;
}
