#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cblog_buffer.h"
#include "cblog_errors.h"

/// <summary>
/// ȷ���������е�ʣ��ռ���ڵ���len
/// ���ʣ��ռ�С��len����ô���Զ�����
/// </summary>
/// <param name="buffer"></param>
/// <param name="len"></param>
/// <returns>0��ʾ�ɹ�������ʧ��</returns>
static int ensure_buffer_size(cblog_buffer *buffer, int len)
{
	// ʣ�໺������С��Ҫ���յ����ݴ�СС�����ݻ�����
	if(buffer->left < len)
	{
		int size1 = buffer->size * 2;
		int size2 = len;
		int newsize = size1 > size2 ? size1 : size2;

		void *newptr = realloc(buffer->pdata, newsize);
		if(newptr == NULL)
		{
			YLOGE("realloc cblog_buffer failed, %s", strerror(errno));
			return 1;
		}

		buffer->pdata = newptr;
		buffer->size = newsize;
		buffer->left = buffer->size - buffer->offset;
	}

	return 0;
}




cblog_buffer *new_cblog_buffer(int size)
{
	cblog_buffer *buffer = (cblog_buffer *)calloc(1, sizeof(cblog_buffer));
	buffer->pdata = (char *)calloc(1, size);
	buffer->size = size;
	buffer->left = size;
	buffer->offset = 0;
	return buffer;
}

void free_cblog_buffer(cblog_buffer *buffer)
{
	free(buffer->pdata);
	free(buffer);
}

void cblog_buffer_reset(cblog_buffer *buffer)
{
	buffer->left = buffer->size;
	buffer->offset = 0;
}

int cblog_buffer_recv_socket(cblog_buffer *buffer, cblog_socket sock)
{
	// ��ȡ��ǰsocket������������ݴ�С
	int avaliable = cblog_socket_get_avaliable_size(sock);
	if(avaliable == -1)
	{
		// ����0���ⲿģ���ر�����
		return 0;
	}

	// ʣ�໺������С��Ҫ���յ����ݴ�СС�����ݻ�����
	if(ensure_buffer_size(buffer, avaliable))
	{
		// ����0���ⲿģ���ر�����
		return 0;
	}

	// ��������
	int rc = recv(sock, buffer->pdata + buffer->offset, avaliable, 0);
	if(rc > 0)
	{
		buffer->offset += rc;
		buffer->left -= rc;
	}
	return rc;
}

int cblog_buffer_write(cblog_buffer *buffer, const char *str, int len)
{
	if(ensure_buffer_size(buffer, len))
	{
		return STEAK_ERR_NO_MEM;
	}

	strncpy(buffer->pdata, str, len);

	return STEAK_ERR_OK;
}