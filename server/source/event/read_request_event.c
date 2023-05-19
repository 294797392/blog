#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#elif (defined(ENV_UNIX))
#endif

#include <libY.h>

#include "steak_socket.h"
#include "errors.h"
#include "event.h"
#include "app.h"
#include "protocol.h"
#include "cblog_string.h"
#include "cblog_types.h"
#include "cblog_sockbuf.h"

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

static void dump_request(cblog_sockbuf *buf, cblog_request *request)
{
	cblog_string_print2("method: ", buf->ptr, &request->method_string);
	cblog_string_print2("uri: ", buf->ptr, &request->url);
	cblog_string_print2("version: ", buf->ptr, &request->version);
	for(int i = 0; i < request->nheader; i++)
	{
		cblog_string_print_header(buf->ptr, &request->headers[i]);
	}
}

static void process_request(svchost *svc, cblog_connection *conn)
{
	cblog_request *request = conn->request;
	cblog_response *response = conn->response;

	dump_request(conn->recvbuf, request);

	switch(request->method)
	{
		case STEAK_HTTP_METHOD_GET:
		{
			//char path[1024] = { '\0' };
			//snprintf(path, sizeof(path), "%s\\%s", svc->options.root, svc->options.index_page);
			//if(Y_file_exist(path) == -1)
			//{
			//	// 文件不存在，显示404
			//	return NULL;
			//}

			//char *content = NULL;
			//uint64_t size = 0;
			//int rc = Y_file_read_all(path, &content, &size);
			//if(rc != YERR_SUCCESS)
			//{
			//	// 文件不存在，显示404
			//	return NULL;
			//}
			break;
		}

		default:
			break;
	}

	return NULL;
}


void http_parser_event_handler(steak_parser *parser, steak_parser_event_enum evt)
{
	cblog_connection *conn = (cblog_connection *)parser->userdata;
	cblog_request *request = conn->request;
	cblog_sockbuf *recvbuf = conn->recvbuf;
	// 原始HTTP报文
	char *raw_msg = recvbuf->ptr;

	switch(evt)
	{
		case STEAK_PARSER_EVENT_METHOD:
		{
			request->method_string.offset = parser->seg_offset;
			request->method_string.length = parser->seg_len;

			size_t len = sizeof(http_method_list) / sizeof(text2enum);
			for(size_t i = 0; i < len; i++)
			{
				if(!cblog_string_casecmp(http_method_list[i].text, raw_msg + parser->seg_offset, parser->seg_len))
				{
					request->method = http_method_list[i].value;
				}
			}
			break;
		}

		case STEAK_PARSER_EVENT_URI:
		{
			request->url.offset = parser->seg_offset;
			request->url.length = parser->seg_len;
			break;
		}

		case STEAK_PARSER_EVENT_VERSION:
		{
			request->version.offset = parser->seg_offset;
			request->version.length = parser->seg_len;
			break;
		}

		case STEAK_PARSER_EVENT_HEADER:
		{
			int nheader = request->nheader;
			request->headers[nheader].key.offset = parser->seg_offset;
			request->headers[nheader].key.length = parser->seg_len;
			request->headers[nheader].value.offset = parser->seg2_offset;
			request->headers[nheader].value.length = parser->seg2_len;
			request->nheader++;
			break;
		}

		case STEAK_PARSER_EVENT_BODY:
		{
			request->body.offset = parser->seg_offset;
			request->body.length = parser->seg_len;
			break;
		}

		default:
			break;
	}
}

int read_request_event(event_module *evm, cblog_event *evt)
{
	cblog_connection *conn = (cblog_connection *)evt->context;
	steak_parser *parser = &conn->parser;
	cblog_sockbuf *recvbuf = conn->recvbuf;
	int old_offset = recvbuf->offset;

	// 接收socket数据
	int rc = cblog_sockbuf_recv(recvbuf);

	// socket返回0，此时对方有两种情况：
	// 1. 调用了close。对方发送了FIN
	// 2. 调用了shutdown(WRITE)
	// 此时说明对方不会再发送数据了
	// 目前还不知道对方到底是调用了close还是shutdown，应该继续尝试发送响应给对方，如果发送也失败，那么说明对方可能是调用了close，那么我们也调用close
	if(rc == 0)
	{
		YLOGE("recv socket zero, close connection");
		Y_queue_enqueue(evm->except_events, evt);
		return STEAK_ERR_OK;
	}

	// socket发生错误, 直接关闭该链接
	if(rc == -1)
	{
		YLOGE("recv socket failed, %d, close connection", steak_socket_error());
		Y_queue_enqueue(evm->except_events, evt);
		return STEAK_ERR_OK;
	}

	// 解析http报文
	// 有可能本次读取的数据里包含下一次的HTTP报文（HTTP流水线模型）
	// 所以使用while循环去解析报文
	while(1)
	{
		int count = steak_parser_parse(parser, recvbuf->ptr, old_offset, rc);
		if(count == -1)
		{
			// 报文还没解析结束，还需要更多数据
			break;
		}
		else if(count == 0)
		{
			// 本次报文接收完毕，并且缓冲区里的数据也都用完了
			// 重置接收缓冲区
			process_request(conn->svc, conn);
			event_modify(evm, evt, evt->read, 1);
			cblog_sockbuf_reset(recvbuf);
			break;
		}
		else
		{
			// 处理的字节数小于接收的字节数，说明本次接收到了新的HTTP请求
			// 把新的HTTP请求报文偏移量移动到接收缓冲区的开头
			process_request(conn->svc, conn);
			event_modify(evm, evt, evt->read, 1);
			memmove(recvbuf->ptr, recvbuf->ptr + old_offset + count, rc - count);
			recvbuf->offset = rc - count;
			recvbuf->left = recvbuf->size - recvbuf->offset;
			// 继续调用parser
			continue;
		}
	}

	return STEAK_ERR_OK;
}
