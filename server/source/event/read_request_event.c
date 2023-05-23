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
#include "cblog_factory.h"
#include "cblog_buffer.h"
#include "cblog_response.h"

static void print_header(cblog_http_header *header)
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
	Y_chain_foreach(cblog_http_header, request->header_chain,
		{
			print_header(current);
		});
}

static void reset_request(cblog_request *request)
{
	Y_chain_foreach(cblog_http_header, request->header_chain, { free_cblog_http_header(current); });
	Y_chain_clear(request->header_chain);
}

static void reset_response(cblog_response *response)
{
	Y_chain_foreach(cblog_http_header, response->header_chain, { free_cblog_http_header(current); });
	Y_chain_clear(response->header_chain);
	cblog_buffer_reset(response->body_buffer);
	cblog_buffer_reset(response->header_buffer);
}

/// <summary>
/// ��HTTP���󱻽������˵���
/// </summary>
/// <param name="evm"></param>
/// <param name="evt"></param>
static void process_request(event_module *evm, cblog_event *evt)
{
	cblog_app *app = cblog_app_get();
	cblog_connection *conn = (cblog_connection *)evt->context;
	cblog_request *request = conn->request;
	cblog_response *response = conn->response;

	// ��ӡ����ĵ�����Ϣ
	//dump_request(request);

	// ����Http����
	cblog_http_context context =
	{
		.request = request,
		.response = response,
		.svc = conn->svc
	};
	app->http_handler->process_request(&context);

	// ����һ���µ�pending_response�������ص������ϣ��ȴ��´���ѯ��ʱ��д��
	cblog_pending_response *pending_response = new_cblog_pending_response(response);
	Y_chain_add(cblog_pending_response, conn->pending_response_list, pending_response);

	// ���ļ�����������Ϊ��д״̬
	event_modify(evm, evt, evt->read, 1);

	// ����request��response��������һ�δ洢
	reset_request(request);
	reset_response(response);
}


void http_parser_event_handler(cblog_parser *parser, cblog_parser_event_enum evt)
{
	cblog_connection *conn = (cblog_connection *)parser->userdata;
	cblog_request *request = conn->request;
	cblog_buffer *recvbuf = conn->recvbuf;

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
			// �����µ�header
			cblog_http_header *header = new_cblog_http_header();
			header->key.offset = parser->seg_offset;
			header->key.length = parser->seg_len;
			header->key.buffer = recvbuf;
			header->value.offset = parser->seg2_offset;
			header->value.length = parser->seg2_len;
			header->value.buffer = recvbuf;

			// �������header����������
			cblog_string *key = &header->key;
			cblog_string *value = &header->value;
			if(!cblog_string_casecmp(key, CBLOG_HTTP_HEADER_STRING_CONNECTION))
			{
				// Connection��ͷ
				if(!cblog_string_casecmp(value, "close") && conn->keep_alive != 0)
				{
					conn->keep_alive = 0;
				}
			}
			else if(!cblog_string_casecmp(key, CBLOG_HTTP_HEADER_STRING_CONTENT_LENGTH))
			{
				// Content-Length��ͷ
				int content_length = cblog_string_to_int32(value);
				// ��parser��content_length�ֶθ�ֵ��������ȷ����HTTP����
				parser->content_length = content_length;
				request->content_length = content_length;
			}

			// header��ӵ�����ĩβ
			Y_chain_add(cblog_http_header, request->header_chain, header);

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
	cblog_buffer *recvbuf = conn->recvbuf;
	int old_offset = recvbuf->offset;

	// ����socket����
	int rc = cblog_buffer_recv_socket(recvbuf, conn->sock);

	// socket����0����ʱ�Է������������
	// 1. ������close���Է�������FIN
	// 2. ������shutdown(WRITE)
	// ��ʱ˵���Է������ٷ���������
	// Ŀǰ����֪���Է������ǵ�����close����shutdown��Ӧ�ü������Է�����Ӧ���Է����������Ҳʧ�ܣ���ô˵���Է������ǵ�����close����ô����Ҳ����close
	if(rc == 0)
	{
		YLOGE("recv socket zero, close connection");
		Y_queue_enqueue(evm->except_events, evt);
		return CBLOG_ERR_OK;
	}

	// socket��������, ֱ�ӹرո�����
	if(rc == -1)
	{
		YLOGE("recv socket failed, %d, close connection", cblog_socket_error());
		Y_queue_enqueue(evm->except_events, evt);
		return CBLOG_ERR_OK;
	}

	// ����http����
	// �п��ܱ��ζ�ȡ�������������һ�ε�HTTP���ģ�HTTP��ˮ��ģ�ͣ�
	// ����ʹ��whileѭ��ȥ��������
	while(1)
	{
		int count = cblog_parser_parse(parser, recvbuf->pdata, old_offset, rc);
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
			cblog_buffer_reset(recvbuf);
			break;
		}
		else
		{
			// ������ֽ���С�ڽ��յ��ֽ�����˵�����ν��յ����µ�HTTP����
			// ���µ�HTTP������ƫ�����ƶ������ջ������Ŀ�ͷ
			process_request(evm, evt);
			memmove(recvbuf->pdata, recvbuf->pdata + old_offset + count, rc - count);
			recvbuf->offset = rc - count;
			recvbuf->left = recvbuf->size - recvbuf->offset;
			// ��������parser
			continue;
		}
	}

	return CBLOG_ERR_OK;
}
