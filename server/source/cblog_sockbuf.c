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
	// ��ȡ��ǰsocket������������ݴ�С
	int avaliable = steak_socket_get_avaliable_size(buffer->sock);
	if(avaliable == -1)
	{
		// ����0���ⲿģ���ر�����
		return 0;
	}

	// ʣ�໺������С��Ҫ���յ����ݴ�СС�����ݻ�����
	if(buffer->left < avaliable)
	{
		int size1 = buffer->size * 2;
		int size2 = avaliable;
		int newsize = size1 > size2 ? size1 : size2;

		buffer->ptr = realloc(buffer->ptr, newsize);
		buffer->size = newsize;
		buffer->left = buffer->size - buffer->offset;
	}

	// ��������
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
