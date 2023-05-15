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
#include "factory.h"

static steak_response *process_request(steak_session *session)
{
	steak_request *request = &session->request;

	switch(request->method)
	{
		case STEAK_HTTP_METHOD_GET:
		{
			char path[1024] = { '\0' };
			snprintf(path, sizeof(path), "%s\\%s", request->url, request->url);
			char *content = NULL;
			uint64_t size = 0;
			int rc = Y_file_read_all(path, &content, &size);
			if(rc != YERR_SUCCESS)
			{

			}
			break;
		}

		default:
			break;
	}

	//char filepath[1024] = { '\0' };
	//snprintf(resuri)

	//switch(request->method)
	//{
	//	case STEAK_HTTP_METHOD_GET:
	//	{
	//		break;
	//	}

	//	default:
	//		break;
	//}
}

int read_request_event(event_module *evm, steak_event *evt)
{
	steak_connection *connection = (steak_connection *)evt->context;
	steak_parser *parser = &connection->parser;
	if(connection->active_session == NULL)
	{
		steak_session *session = new_session(evt->sock);
		connection->active_session = session;
	}
	steak_session *active_session = connection->active_session;

	// 准备接收缓冲区
	char *recv_buf = parser->raw_msg + parser->raw_msg_offset;
	int recv_buf_size = parser->raw_msg_len - parser->raw_msg_offset;

	// 获取当前socket缓冲区里的数据大小
	int data_size = steak_socket_get_avaliable_size(evt->sock);
	if(data_size == -1)
	{
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
		return STEAK_ERR_OK;
	}
	else if(rc == -1)
	{
		// socket发生错误, 把event标记为无效，后面删除event
		return STEAK_ERR_OK;
	}
	else
	{
		// 读到了一些字节数
		// 解析http报文
		parser->readsize = data_size;
		steak_parser_parse(parser, &active_session->request);
		switch(parser->state)
		{
			case STEAK_PARSER_COMPLETED:
			{
				YLOGI("http request receive completed, begin process request");

				// 当前session设置为空，说明本次请求已经解析完了
				// 下次收到数据的时候直接创建新的session
				connection->active_session = NULL;

				// 将active_session加到待发送会话的队尾
				if(connection->last_session == NULL)
				{
					connection->first_session = active_session;
					connection->last_session = active_session;
				}
				else
				{
					connection->last_session->next = active_session;
					active_session->prev = connection->last_session;
					connection->last_session = active_session;
				}

				process_request(&active_session);

				break;
			}

			case STEAK_PARSER_ERROR:
			{
				YLOGE("http request parse error, invalid http request");
				break;
			}

			default:
				break;
		}
	}

	return STEAK_ERR_OK;
}
