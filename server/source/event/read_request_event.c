#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#elif (defined(ENV_UNIX))
#endif

#include <libY.h>

#include "protocol.h"
#include "cblog_app.h"
#include "cblog_errors.h"
#include "cblog_socket.h"
#include "cblog_string.h"
#include "cblog_types.h"
#include "cblog_parser.h"
#include "cblog_event_module.h"

typedef struct textvalue_s
{
	char *text;
	int value;
}textvalue;

static textvalue http_method_list[] =
{
	{.text = "GET",		.value = STEAK_HTTP_METHOD_GET },
	{.text = "POST",	.value = STEAK_HTTP_METHOD_POST },
	{.text = "HEAD",	.value = STEAK_HTTP_METHOD_HEAD },
	{.text = "PUT",		.value = STEAK_HTTP_METHOD_PUT },
	{.text = "TRACE",	.value = STEAK_HTTP_METHOD_TRACE },
	{.text = "OPTIONS", .value = STEAK_HTTP_METHOD_OPTIONS },
	{.text = "DELETE",	.value = STEAK_HTTP_METHOD_DELETE },
	{.text = NULL }
};

static textvalue http_version_list[] =
{
	{.text = "HTTP/1.1", .value = CBLOG_HTTP_VERSION_1DOT1 },
	{.text = NULL }
};

static void dump_request(cblog_request *request)
{
	cblog_string_print2("method: ", &request->method_string);
	cblog_string_print2("uri: ", &request->url);
	cblog_string_print2("version: ", &request->version_string);
	for(int i = 0; i < request->nheader; i++)
	{
		cblog_string_print_header(&request->headers[i]);
	}
}

static int text2value(cblog_string *str, textvalue *tvs)
{
	for(int i = 0; ; i++)
	{
		textvalue *tv = &tvs[i];

		if(tv->text == NULL)
		{
			return 0;
		}

		if(!cblog_string_casecmp(str, tv->text))
		{
			return tv->value;
		}
	}

	return 0;
}

static void process_request(event_module *evm, cblog_event *evt)
{
	cblog_connection *conn = (cblog_connection *)evt->context;
	cblog_request *request = conn->request;
	cblog_response *response = conn->response;

	dump_request(request);

	event_modify(evm, evt, evt->read, 1);

	switch(request->method)
	{
		case STEAK_HTTP_METHOD_GET:
		{
			//char path[1024] = { '\0' };
			//snprintf(path, sizeof(path), "%s\\%s", svc->options.root, svc->options.index_page);
			//if(Y_file_exist(path) == -1)
			//{
			//	// �ļ������ڣ���ʾ404
			//	return NULL;
			//}

			//char *content = NULL;
			//uint64_t size = 0;
			//int rc = Y_file_read_all(path, &content, &size);
			//if(rc != YERR_SUCCESS)
			//{
			//	// �ļ������ڣ���ʾ404
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
	cblog_socket_buffer *recvbuf = conn->recvbuf;

	switch(evt)
	{
		case STEAK_PARSER_EVENT_METHOD:
		{
			request->method_string.offset = parser->seg_offset;
			request->method_string.length = parser->seg_len;
			request->method_string.buffer = recvbuf;
			request->method = text2value(&request->method_string, http_method_list);
			break;
		}

		case STEAK_PARSER_EVENT_URI:
		{
			request->url.offset = parser->seg_offset;
			request->url.length = parser->seg_len;
			request->url.buffer = recvbuf;
			break;
		}

		case STEAK_PARSER_EVENT_VERSION:
		{
			request->version_string.offset = parser->seg_offset;
			request->version_string.length = parser->seg_len;
			request->version_string.buffer = recvbuf;
			request->version = text2value(&request->version_string, http_version_list);
			break;
		}

		case STEAK_PARSER_EVENT_HEADER:
		{
			int nheader = request->nheader;
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
				// Connection��ͷ
				if(!cblog_string_casecmp(value, "close") && conn->keep_alive != 0)
				{
					conn->keep_alive = 0;
				}
			}
			else if(!cblog_string_casecmp(key, CBLOG_HTTP_HEADER_CONTENT_LENGTH))
			{
				// Content-Length��ͷ
				int content_length = cblog_string_to_int32(value);
				// ��parser��content_length�ֶθ�ֵ��������ȷ����HTTP����
				parser->content_length = content_length;
				request->content_length = content_length;
			}

			request->nheader++;

			break;
		}

		case STEAK_PARSER_EVENT_BODY:
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
	steak_parser *parser = &conn->parser;
	cblog_socket_buffer *recvbuf = conn->recvbuf;
	int old_offset = recvbuf->offset;

	// ����socket����
	int rc = cblog_sockbuf_recv(recvbuf);

	// socket����0����ʱ�Է������������
	// 1. ������close���Է�������FIN
	// 2. ������shutdown(WRITE)
	// ��ʱ˵���Է������ٷ���������
	// Ŀǰ����֪���Է������ǵ�����close����shutdown��Ӧ�ü������Է�����Ӧ���Է����������Ҳʧ�ܣ���ô˵���Է������ǵ�����close����ô����Ҳ����close
	if(rc == 0)
	{
		YLOGE("recv socket zero, close connection");
		Y_queue_enqueue(evm->except_events, evt);
		return STEAK_ERR_OK;
	}

	// socket��������, ֱ�ӹرո�����
	if(rc == -1)
	{
		YLOGE("recv socket failed, %d, close connection", steak_socket_error());
		Y_queue_enqueue(evm->except_events, evt);
		return STEAK_ERR_OK;
	}

	// ����http����
	// �п��ܱ��ζ�ȡ�������������һ�ε�HTTP���ģ�HTTP��ˮ��ģ�ͣ�
	// ����ʹ��whileѭ��ȥ��������
	while(1)
	{
		int count = steak_parser_parse(parser, recvbuf->ptr, old_offset, rc);
		if(count == -1)
		{
			// ���Ļ�û��������������Ҫ��������
			recvbuf->offset += rc;
			break;
		}
		else if(count == 0)
		{
			// ���α��Ľ�����ϣ����һ������������Ҳ��������
			// ���ý��ջ�����
			process_request(evm, evt);
			cblog_sockbuf_reset(recvbuf);
			break;
		}
		else
		{
			// ������ֽ���С�ڽ��յ��ֽ�����˵�����ν��յ����µ�HTTP����
			// ���µ�HTTP������ƫ�����ƶ������ջ������Ŀ�ͷ
			process_request(evm, evt);
			memmove(recvbuf->ptr, recvbuf->ptr + old_offset + count, rc - count);
			recvbuf->offset = rc - count;
			recvbuf->left = recvbuf->size - recvbuf->offset;
			// ��������parser
			continue;
		}
	}

	return STEAK_ERR_OK;
}
