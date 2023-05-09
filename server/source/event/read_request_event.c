#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>

#include <libY.h>

#include "errors.h"
#include "event.h"
#include "app.h"

int read_request_event(event_module *evm, steak_event *evt)
{
	steak_session *session = (steak_session *)evt->context;
	steak_request *request = session->request;

	char *recv_buf = request->buffer + request->write_offset;
	int recv_buf_size = request->buffer_size - request->write_offset;

	int rc = recv(evt->sock, recv_buf, recv_buf_size, 0);
	if(rc == 0)
	{
		// 此时说明对方关闭了该网络通道的读取（调用shutdown(READ)）
		// 收到了FIN，此时返回EOF
		// 那么此时删除该读取事件
		evt->readable = 0;
	}
	else if(rc == -1)
	{
		// socket发生错误
		evt->readable = 0;
		evt->writeable = 0;
	}
	else
	{
		// 读到了一些字节数

		// 如果缓冲区读取位置达到了缓冲区末尾，那么就扩容
		if(request->write_offset == request->buffer_size)
		{
			int newsize = request->buffer_size * 2;
			request->buffer = (char *)realloc(request->buffer, newsize);
			request->buffer_size = newsize;
		}

		request->write_offset += rc;
	}

	// 解析http报文

	return STEAK_ERR_FAILED;
}
