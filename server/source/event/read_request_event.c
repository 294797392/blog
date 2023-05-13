#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <WinSock2.h>

#include <libY.h>

#include "steak_socket.h"
#include "errors.h"
#include "event.h"
#include "app.h"

int read_request_event(event_module *evm, steak_event *evt)
{
	steak_connection *connection = (steak_connection *)evt->context;
	steak_parser *parser = &connection->parser;

	// 准备接收缓冲区
	char *recv_buf = parser->raw_msg + parser->raw_msg_offset;
	int recv_buf_size = parser->raw_msg_len - parser->raw_msg_offset;

	// 获取当前socket缓冲区里的数据大小
	int data_size = steak_socket_get_avaliable_size(evt->sock);
	if(data_size == -1)
	{
		evt->status = STEAK_EVENT_STATUS_DELETE;
		return STEAK_ERR_OK;
	}

	// 要接收的数据比缓冲区大
	// 扩容缓冲区
	if(recv_buf_size < data_size)
	{
		int size1 = parser->raw_msg_offset + data_size;
		int size2 = parser->raw_msg_len * 2;
		int newsize = size1 > size2 ? size1 : size2;

		parser->raw_msg = Y_pool_resize(parser->raw_msg, parser->raw_msg_len, newsize);
		parser->raw_msg_len = newsize;

		recv_buf = parser->raw_msg + parser->raw_msg_offset;
	}

	// 接收数据
	int rc = recv(evt->sock, recv_buf, data_size, 0);
	if(rc == 0)
	{
		// 此时说明对方关闭了该网络通道的读取（调用shutdown(READ)）
		// 收到了FIN，此时返回EOF
		evt->status = STEAK_EVENT_STATUS_DELETE;
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
		// 解析http报文
		parser->readsize = data_size;
		steak_parser_parse(parser);

		if(parser->completed)
		{
			YLOGI("http request receive completed");
		}
	}

	return STEAK_ERR_OK;
}
