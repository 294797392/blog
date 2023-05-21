#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#elif (defined(ENV_UNIX))
#endif

#include <libY.h>

#include "cblog_protocol.h"
#include "cblog_app.h"
#include "cblog_errors.h"
#include "cblog_socket.h"
#include "cblog_string.h"
#include "cblog_types.h"
#include "cblog_parser.h"
#include "cblog_event_module.h"

static print_header(cblog_http_header *header)
{
	cblog_string *key = &header->key;
	cblog_string *value = &header->value;

	char format[64] = { '\0' };
	snprintf(format, sizeof(format), "%%.%ds: %%.%ds\r\n", header->key.length, header->value.length);
	printf(format, cblog_string_string(key), cblog_string_string(value));
}

static void dump_request(cblog_request *request)
{
	cblog_string_print2("method: ", &request->method_string);
	cblog_string_print2("uri: ", &request->url);
	cblog_string_print2("version: ", &request->version_string);
	for(int i = 0; i < request->nheader; i++)
	{
		print_header(&request->headers[i]);
	}
}

static void generate_response(cblog_request *request, cblog_response *response)
{
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
}

/// <summary>
/// 当HTTP请求被接收完了调用
/// </summary>
/// <param name="evm"></param>
/// <param name="evt"></param>
static void process_request(event_module *evm, cblog_event *evt)
{
	cblog_connection *conn = (cblog_connection *)evt->context;
	cblog_request *request = conn->request;
	cblog_response *response = conn->response;

	// 打印请求的调试信息
	dump_request(request);

	// 生成响应
	generate_response(request, response);

	// 把文件描述符设置为可写状态
	event_modify(evm, evt, evt->read, 1);

	request->nheader = 0;
}


void http_parser_event_handler(cblog_parser *parser, cblog_parser_event_enum evt)
{
	cblog_connection *conn = (cblog_connection *)parser->userdata;
	cblog_request *request = conn->request;
	cblog_socket_buffer *recvbuf = conn->recvbuf;

	switch(evt)
	{
		case CBLOG_PARSER_EVENT_METHOD:
		{
			request->method_string.offset = parser->seg_offset;
			request->method_string.length = parser->seg_len;
			request->method_string.buffer = recvbuf;
			request->method = cblog_http_method_string2enum(&request->method_string);
			break;
		}

		case CBLOG_PARSER_EVENT_URI:
		{
			request->url.offset = parser->seg_offset;
			request->url.length = parser->seg_len;
			request->url.buffer = recvbuf;
			break;
		}

		case CBLOG_PARSER_EVENT_VERSION:
		{
			request->version_string.offset = parser->seg_offset;
			request->version_string.length = parser->seg_len;
			request->version_string.buffer = recvbuf;
			request->version = cblog_http_version_string2enum(&request->version_string);
			break;
		}

		case CBLOG_PARSER_EVENT_HEADER:
		{
			int nheader = request->nheader;
			if(nheader == request->max_header)
			{
				// TODO：
				// 这里需要扩容，因为目前用的不是指针，先暂不处理
				// 记录日志以后处理
				YLOGE("too many header, ignore");
				break;
			}

			request->headers[nheader].key.offset = parser->seg_offset;
			request->headers[nheader].key.length = parser->seg_len;
			request->headers[nheader].key.buffer = recvbuf;
			request->headers[nheader].value.offset = parser->seg2_offset;
			request->headers[nheader].value.length = parser->seg2_len;
			request->headers[nheader].value.buffer = recvbuf;

			cblog_string *key = &request->headers[nheader].key;
			cblog_string *value = &request->headers[nheader].value;

			if(!cblog_string_casecmp(key, CBLOG_HTTP_HEADER_CONNECTION))
			{
				// Connection标头
				if(!cblog_string_casecmp(value, "close") && conn->keep_alive != 0)
				{
					conn->keep_alive = 0;
				}
			}
			else if(!cblog_string_casecmp(key, CBLOG_HTTP_HEADER_CONTENT_LENGTH))
			{
				// Content-Length标头
				int content_length = cblog_string_to_int32(value);
				// 给parser的content_length字段赋值，才能正确解析HTTP报文
				parser->content_length = content_length;
				request->content_length = content_length;
			}

			request->nheader++;

			break;
		}

		case CBLOG_PARSER_EVENT_BODY:
		{
			request->body.offset = parser->seg_offset;
			request->body.length = parser->seg_len;
			request->body.buffer = recvbuf;
			break;
		}

		default:
			break;
	}
}

int read_request_event(event_module *evm, cblog_event *evt)
{
	cblog_connection *conn = (cblog_connection *)evt->context;
	cblog_parser *parser = &conn->parser;
	cblog_socket_buffer *recvbuf = conn->recvbuf;
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
		YLOGE("recv socket failed, %d, close connection", cblog_socket_error());
		Y_queue_enqueue(evm->except_events, evt);
		return STEAK_ERR_OK;
	}

	// 解析http报文
	// 有可能本次读取的数据里包含下一次的HTTP报文（HTTP流水线模型）
	// 所以使用while循环去解析报文
	while(1)
	{
		int count = cblog_parser_parse(parser, recvbuf->ptr, old_offset, rc);
		if(count == -1)
		{
			// 报文还没解析结束，还需要更多数据
			recvbuf->offset += rc;
			break;
		}
		else if(count == 0)
		{
			// 本次报文接收完毕，并且缓冲区里的数据也都用完了
			// 重置接收缓冲区
			process_request(evm, evt);
			cblog_sockbuf_reset(recvbuf);
			break;
		}
		else
		{
			// 处理的字节数小于接收的字节数，说明本次接收到了新的HTTP请求
			// 把新的HTTP请求报文偏移量移动到接收缓冲区的开头
			process_request(evm, evt);
			memmove(recvbuf->ptr, recvbuf->ptr + old_offset + count, rc - count);
			recvbuf->offset = rc - count;
			recvbuf->left = recvbuf->size - recvbuf->offset;
			// 继续调用parser
			continue;
		}
	}

	return STEAK_ERR_OK;
}
