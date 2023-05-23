#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cblog_buffer.h"
#include "cblog_errors.h"

/// <summary>
/// 确保缓冲区中的剩余空间大于等于len
/// 如果剩余空间小于len，那么会自动扩容
/// </summary>
/// <param name="buffer"></param>
/// <param name="len"></param>
/// <returns>0表示成功，否则失败</returns>
static int ensure_buffer_size(cblog_buffer *buffer, int len)
{
	// 剩余缓冲区大小比要接收的数据大小小，扩容缓冲区
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
	// 获取当前socket缓冲区里的数据大小
	int avaliable = cblog_socket_get_avaliable_size(sock);
	if(avaliable == -1)
	{
		// 返回0，外部模块会关闭连接
		return 0;
	}

	// 剩余缓冲区大小比要接收的数据大小小，扩容缓冲区
	if(ensure_buffer_size(buffer, avaliable))
	{
		// 返回0，外部模块会关闭连接
		return 0;
	}

	// 接收数据
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
	if(len == 0)
	{
		return CBLOG_ERR_OK;
	}

	if(ensure_buffer_size(buffer, len))
	{
		return CBLOG_ERR_NO_MEM;
	}

	strncpy(buffer->pdata + buffer->offset, str, len);
	buffer->offset += len;
	buffer->left -= len;

	return CBLOG_ERR_OK;
}

int cblog_buffer_write2(cblog_buffer *writeto, cblog_buffer *buffer2)
{
	int len = buffer2->offset;
	if(len == 0)
	{
		return CBLOG_ERR_OK;
	}

	if(ensure_buffer_size(writeto, len))
	{
		return CBLOG_ERR_NO_MEM;
	}

	strncpy(writeto->pdata + writeto->offset, buffer2->pdata, len);
	writeto->offset += len;
	writeto->left -= len;

	return CBLOG_ERR_OK;
}

int cblog_buffer_write3(cblog_buffer *buffer, cblog_string *str)
{
	int len = str->length;
	if(len == 0)
	{
		return CBLOG_ERR_OK;
	}

	if(ensure_buffer_size(buffer, len))
	{
		return CBLOG_ERR_NO_MEM;
	}

	strncpy(buffer->pdata + buffer->offset, str->buffer->pdata + str->offset, len);
	buffer->offset += len;
	buffer->left -= len;

	return CBLOG_ERR_OK;
}