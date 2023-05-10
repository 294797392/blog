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
		return STEAK_ERR_OK;
	}
	else if(rc == -1)
	{
		// socket发生错误, 把event标记为无效，后面删除event
		evt->status = STEAK_EVENT_STATUS_DELETE;
		return STEAK_ERR_OK;
	}
	else
	{
		// 读到了一些字节数

		// 如果缓冲区读取位置达到了缓冲区末尾，那么就扩容
		if(request->write_offset == request->buffer_size)
		{
			int newsize = request->buffer_size * 2;
			char *buffer = (char *)realloc(request->buffer, newsize);
			if(buffer == NULL)
			{
				YLOGE("realloc request buffer failed, %s", strerror(errno));
				return STEAK_ERR_NO_MEM;
			}
			request->buffer = buffer;
			request->buffer_size = newsize;
		}

		request->write_offset += rc;
	}

	// 解析http报文
	//steak_parser_parse(request);

	return STEAK_ERR_FAILED;
}
