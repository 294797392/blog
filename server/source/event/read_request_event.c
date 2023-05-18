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
#include "steak_string.h"
#include "errors.h"
#include "event.h"
#include "app.h"
#include "protocol.h"
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

static void process_request(svchost *svc, steak_connection *conn)
{
	steak_request *request = conn->request;
	steak_response *response = conn->response;

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
	steak_connection *conn = (steak_connection *)parser->userdata;
	steak_request *request = conn->request;
	cblog_sockbuf *recvbuf = conn->recvbuf;
	// ԭʼHTTP����
	char *raw_msg = recvbuf->ptr;

	switch(evt)
	{
		case STEAK_PARSER_EVENT_METHOD:
		{
			size_t len = sizeof(http_method_list) / sizeof(text2enum);
			for(size_t i = 0; i < len; i++)
			{
				if(!strcasecmp(http_method_list[i].text, raw_msg + parser->seg_offset, parser->seg_len))
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

int read_request_event(event_module *evm, steak_event *evt)
{
	steak_connection *conn = (steak_connection *)evt->context;
	steak_parser *parser = &conn->parser;
	cblog_sockbuf *recvbuf = conn->recvbuf;
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
		steak_socket_close(evt->sock);
		event_remove(evm, evt);
		free_connection_event(evm, evt);
		return STEAK_ERR_OK;
	}

	// socket��������, ֱ�ӹرո�����
	if(rc == -1)
	{
		YLOGE("recv socket failed, %s, close connection", strerror(errno));
		steak_socket_close(evt->sock);
		event_remove(evm, evt);
		free_connection_event(evm, evt);
		return STEAK_ERR_OK;
	}

	// ����http����
	// �п��ܱ��ζ�ȡ�������������һ�ε�HTTP���ģ�HTTP��ˮ��ģ�ͣ�
	// ����ʹ��whileѭ��ȥ��������
	while(1)
	{
		int count = steak_parser_parse(parser, recvbuf->ptr, old_offset, rc);
		if(count == 0)
		{
			// ���α��Ľ�����ϣ����һ������������Ҳ��������
			// ���ý��ջ�����
			process_request(conn->svc, conn);
			event_modify(evm, evt, evt->read, 1);
			cblog_sockbuf_reset(recvbuf);
			break;
		}
		else if(count < rc)
		{
			// ������ֽ���С�ڽ��յ��ֽ�����˵�����ν��յ����µ�HTTP����
			// ���µ�HTTP������ƫ�����ƶ������ջ������Ŀ�ͷ
			process_request(conn->svc, conn);
			event_modify(evm, evt, evt->read, 1);
			memmove(recvbuf->ptr, recvbuf->ptr + old_offset + count, rc - count);
			recvbuf->offset = rc - count;
			recvbuf->left = recvbuf->size - recvbuf->offset;
			// ��������parser
			continue;
		}
		else if(count == rc)
		{
			// ���α��Ļ�û������ϣ���Ҫ�������ղ�����
			break;
		}
		else
		{
			// count > recv_len�������ܳ���
			YLOGE("http parser result = %d, size = %d", count, rc);
			break;
		}
	}

	return STEAK_ERR_OK;
}
