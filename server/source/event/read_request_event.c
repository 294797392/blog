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
				// �ļ������ڣ���ʾ404
				return NULL;
			}

			char *content = NULL;
			uint64_t size = 0;
			int rc = Y_file_read_all(path, &content, &size);
			if(rc != YERR_SUCCESS)
			{
				// �ļ������ڣ���ʾ404
				return NULL;
			}

			break;
		}

		default:
			break;
	}

	return NULL;
}





void http_parser_event_handler(steak_parser *parser, steak_parser_event_enum evt, char *data1, int data1len, char *data2, int data2len)
{
	steak_connection *conn = (steak_connection *)parser->userdata;
	steak_session *active_session = conn->active_session;
	steak_request *request = &active_session->request;

	switch(evt)
	{
		case STEAK_PARSER_EVENT_METHOD:
		{
			size_t len = sizeof(http_method_list) / sizeof(text2enum);
			for(size_t i = 0; i < len; i++)
			{
				if(!strcasecmp(http_method_list[i].text, data1, data1len))
				{
					request->method = http_method_list[i].value;
				}
			}
			YLOGI("method = %d", request->method);
			break;
		}

		case STEAK_PARSER_EVENT_URI:
		{
			request->url = (char *)Y_pool_obtain(data1len + 1);
			strncpy(request->url, data1, data1len);
			YLOGI("uri = %s", request->url);
			break;
		}

		case STEAK_PARSER_EVENT_VERSION:
		{
			request->version = (char *)Y_pool_obtain(data1len + 1);
			strncpy(request->version, data1, data1len);
			YLOGI("http_version = %s", request->version);
			break;
		}

		case STEAK_PARSER_EVENT_HEADER:
		{
			steak_http_header *header = (steak_http_header *)Y_pool_obtain(sizeof(steak_http_header));
			header->key = (char *)Y_pool_obtain(data1len + 1);
			strncpy(header->key, data1, data1len);
			header->value = (char *)Y_pool_obtain(data2len + 1);
			strncpy(header->value, data2, data2len);

			if(request->first_header == NULL)
			{
				request->first_header = header;
				request->last_header = header;
			}
			else
			{
				request->last_header->next = header;
				header->prev = request->last_header;
				request->last_header = header;
			}
			YLOGI("%s = %s", header->key, header->value);
			break;
		}

		case STEAK_PARSER_EVENT_BODY:
		{
			request->body = (char *)Y_pool_obtain(data1len + 1);
			strncpy(request->body, data1, data1len);
			YLOGI("body = %s", request->body);
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
	if(conn->active_session == NULL)
	{
		conn->active_session = (steak_session *)Y_pool_obtain(sizeof(steak_session));
	}
	steak_session *active_session = conn->active_session;

	// ��ȡ��ǰsocket������������ݴ�С
	int recv_len = steak_socket_get_avaliable_size(evt->sock);
	if(recv_len == -1)
	{
		return STEAK_ERR_OK;
	}

	// ʣ�໺������С��Ҫ���յ����ݴ�СС�����ݻ�����
	int left_buf_len = conn->recv_buf_len - conn->recv_buf_offset;
	if(left_buf_len < recv_len)
	{
		int size1 = conn->recv_buf_offset + recv_len;
		int size2 = conn->recv_buf_len * 2;
		int newsize = size1 > size2 ? size1 : size2;

		conn->recv_buf = Y_pool_resize(conn->recv_buf, conn->recv_buf_len, newsize);
		conn->recv_buf_len = newsize;
	}

	// ���ջ�����
	char *recv_buf = conn->recv_buf + conn->recv_buf_offset;

	// ��������
	int rc = recv(evt->sock, recv_buf, recv_len, 0);
	if(rc == 0)
	{
		// ��ʱ�Է������������
		// 1. ������close���Է�������FIN
		// 2. ������shutdown(WRITE)
		// ��ʱ˵���Է������ٷ���������
		// Ŀǰ����֪���Է������ǵ�����close����shutdown��Ӧ�ü������Է�����Ӧ���Է����������Ҳʧ�ܣ���ô˵���Է������ǵ�����close����ô����Ҳ����close
		YLOGE("read socket zero, close read");
		event_modify(evm, evt, 0, evt->write);
		return STEAK_ERR_OK;
	}
	else if(rc == -1)
	{
		// socket��������, ֱ�ӹرո�����
		YLOGE("read socket failed, %s, close connection", strerror(errno));
		steak_socket_close(evt->sock);
		event_delete(evm, evt);
		free_connection_event(evm, evt);
		return STEAK_ERR_OK;
	}
	else
	{
		// ����http����
		int count = 0;
	parse:
		count = steak_parser_parse(parser, conn->recv_buf, conn->recv_buf_offset, recv_len);
		if(count == 0)
		{
			// ���α��Ľ�����ϣ����һ������������Ҳ��������
			// ���ý��ջ�����
			conn->recv_buf_offset = 0;
			process_request(conn->svc, active_session);
			conn->active_session = NULL;
		}
		else if(count < recv_len)
		{
			// ������ֽ���С�ڽ��յ��ֽ�����˵�����ν��յ����µ�HTTP����
			// ���µ�HTTP������ƫ�����ƶ������ջ������Ŀ�ͷ
			conn->recv_buf_offset = 0;
			conn->recv_buf_len = recv_len - count;
			memmove(conn->recv_buf, conn->recv_buf + conn->recv_buf_offset + count, conn->recv_buf_len);
			process_request(conn->svc, active_session);
			conn->active_session = (steak_session *)Y_pool_obtain(sizeof(steak_session));

			// ��������parser
			goto parse;
		}
		else if(count == recv_len)
		{
			// ���α��Ļ�û������ϣ���Ҫ�������ղ�����
		}
		else
		{
			// count > recv_len�������ܳ���
			YLOGE("http parser result = %d, size = %d", count, recv_len);
		}

		//switch(parser->state)
		//{
		//	case STEAK_PARSER_COMPLETED:
		//	{
		//		YLOGI("http request receive completed, begin process request");

		//		// ��ǰsession����Ϊ�գ�˵�����������Ѿ���������
		//		// �´��յ����ݵ�ʱ��ֱ�Ӵ����µ�session
		//		conn->active_session = NULL;

		//		// ��active_session�ӵ������ͻỰ�Ķ�β
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
