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
#include "protocol.h"

typedef struct text2enum_s
{
	char *text;
	int value;
}text2enum;

static text2enum http_method_list[] =
{
	{.text = "GET",		.value = STEAK_HTTP_METHOD_GET },
	{.text = "POST",	.value = STEAK_HTTP_METHOD_POST },
	{.text = "HEAD",	.value = STEAK_HTTP_METHOD_HEAD },
	{.text = "PUT",		.value = STEAK_HTTP_METHOD_PUT },
	{.text = "TRACE",	.value = STEAK_HTTP_METHOD_TRACE },
	{.text = "OPTIONS", .value = STEAK_HTTP_METHOD_OPTIONS },
	{.text = "DELETE",	.value = STEAK_HTTP_METHOD_DELETE }
};

static steak_response *process_request(svchost *svc, steak_session *session)
{
	steak_request *request = &session->request;
	steak_response *response = &session->response;

	switch(request->method)
	{
		case STEAK_HTTP_METHOD_GET:
		{
			char path[1024] = { '\0' };
			snprintf(path, sizeof(path), "%s\\%s", svc->options.root, svc->options.index_page);
			if(Y_file_exist(path) == -1)
			{
				// 文件不存在，显示404
				return NULL;
			}

			char *content = NULL;
			uint64_t size = 0;
			int rc = Y_file_read_all(path, &content, &size);
			if(rc != YERR_SUCCESS)
			{
				// 文件不存在，显示404
				return NULL;
			}

			break;
		}

		default:
			break;
	}

	return NULL;
}

static void on_parser_event(steak_parser *parser, steak_parser_event_enum evt, char *data1, int data1len, char *data2, int data2len)
{
	switch(evt)
	{
		case STEAK_PARSER_EVENT_METHOD:
		{
			//size_t len = sizeof(http_method_list) / sizeof(text2enum);
			//for(size_t i = 0; i < len; i++)
			//{
			//	if(!strcasecmp(http_method_list[i].text, method, method_len))
			//	{
			//		request->method = http_method_list[i].value;
			//	}
			//}
			break;
		}

		case STEAK_PARSER_EVENT_URI:
		{
			break;
		}

		case STEAK_PARSER_EVENT_VERSION:
		{
			break;
		}

		case STEAK_PARSER_EVENT_HEADER:
		{
			break;
		}

		case STEAK_PARSER_EVENT_BODY: 
		{
			break;
		}

		default:
			break;
	}
}




int read_request_event(event_module *evm, steak_event *evt)
{
	steak_connection *conn = (steak_connection *)evt->context;
	steak_parser *parser = &conn->parser;
	parser->on_event = on_parser_event;
	// 要解析的HTTP报文偏移
	char *recv_buf = NULL;

	// 剩余的缓冲区大小
	int remain_buf_size = conn->recv_buf_len - conn->recv_buf_offset;

	// 获取当前socket缓冲区里的数据大小
	int recv_len = steak_socket_get_avaliable_size(evt->sock);
	if(recv_len == -1)
	{
		return STEAK_ERR_OK;
	}

	// 剩余缓冲区大小比要接收的数据大小小，扩容缓冲区
	if(remain_buf_size < recv_len)
	{
		int size1 = conn->recv_buf_offset + recv_len;
		int size2 = conn->recv_buf_len * 2;
		int newsize = size1 > size2 ? size1 : size2;

		conn->recv_buf = Y_pool_resize(conn->recv_buf, conn->recv_buf_len, newsize);
		conn->recv_buf_len = newsize;
	}

	recv_buf = conn->recv_buf + conn->recv_buf_offset;

	// 接收数据
	int rc = recv(evt->sock, recv_buf, recv_len, 0);
	if(rc == 0)
	{
		// 此时对方有两种情况：
		// 1. 调用了close。对方发送了FIN
		// 2. 调用了shutdown(WRITE)
		// 此时说明对方不会再发送数据了
		// 目前还不知道对方到底是调用了close还是shutdown，应该继续尝试发送响应给对方，如果发送也失败，那么说明对方可能是调用了close，那么我们也调用close
		YLOGE("read socket zero, close read");
		event_modify(evm, evt, 0, evt->write);
		return STEAK_ERR_OK;
	}
	else if(rc == -1)
	{
		// socket发生错误, 直接关闭该链接
		YLOGE("read socket failed, %s, close connection", strerror(errno));
		steak_socket_close(evt->sock);
		event_delete(evm, evt);
		free_connection_event(evm, evt);
		return STEAK_ERR_OK;
	}
	else
	{
		// 解析http报文
		int count = 0;
parse:
		count = steak_parser_parse(parser, conn->recv_buf, conn->recv_buf_offset, recv_len);
		if(count == 0)
		{
			// 本次报文接收完毕，并且缓冲区里的数据也都用完了
			// 重置接收缓冲区
			conn->recv_buf_offset = 0;
			process_request(conn->svc, NULL);
		}
		else if(count < recv_len)
		{
			// 处理的字节数小于接收的字节数，说明本次接收到了新的HTTP请求
			// 把新的HTTP请求报文偏移量移动到接收缓冲区的开头
			conn->recv_buf_offset = 0;
			conn->recv_buf_len = recv_len - count;
			memmove(conn->recv_buf, conn->recv_buf + conn->recv_buf_offset + count, conn->recv_buf_len);

			process_request(conn->svc, NULL);

			// 继续调用parser
			goto parse;
		}
		else if(count == recv_len)
		{
			// 本次报文还没解析完毕，需要继续接收并解析
		}
		else
		{
			// count > recv_len，不可能出现
			YLOGE("http parser result = %d, size = %d", count, recv_len);
		}

		//switch(parser->state)
		//{
		//	case STEAK_PARSER_COMPLETED:
		//	{
		//		YLOGI("http request receive completed, begin process request");

		//		// 当前session设置为空，说明本次请求已经解析完了
		//		// 下次收到数据的时候直接创建新的session
		//		conn->active_session = NULL;

		//		// 将active_session加到待发送会话的队尾
		//		if(conn->last_session == NULL)
		//		{
		//			conn->first_session = active_session;
		//			conn->last_session = active_session;
		//		}
		//		else
		//		{
		//			conn->last_session->next = active_session;
		//			active_session->prev = conn->last_session;
		//			conn->last_session = active_session;
		//		}

		//		process_request(conn, active_session);

		//		break;
		//	}

		//	case STEAK_PARSER_ERROR:
		//	{
		//		YLOGE("http request parse error, invalid http request");
		//		break;
		//	}

		//	default:
		//		break;
		//}
	}

	return STEAK_ERR_OK;
}
