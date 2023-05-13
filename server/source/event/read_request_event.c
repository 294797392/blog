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

	// ׼�����ջ�����
	char *recv_buf = parser->raw_msg + parser->raw_msg_offset;
	int recv_buf_size = parser->raw_msg_len - parser->raw_msg_offset;

	// ��ȡ��ǰsocket������������ݴ�С
	int data_size = steak_socket_get_avaliable_size(evt->sock);
	if(data_size == -1)
	{
		evt->status = STEAK_EVENT_STATUS_DELETE;
		return STEAK_ERR_OK;
	}

	// Ҫ���յ����ݱȻ�������
	// ���ݻ�����
	if(recv_buf_size < data_size)
	{
		int size1 = parser->raw_msg_offset + data_size;
		int size2 = parser->raw_msg_len * 2;
		int newsize = size1 > size2 ? size1 : size2;

		parser->raw_msg = Y_pool_resize(parser->raw_msg, parser->raw_msg_len, newsize);
		parser->raw_msg_len = newsize;

		recv_buf = parser->raw_msg + parser->raw_msg_offset;
	}

	// ��������
	int rc = recv(evt->sock, recv_buf, data_size, 0);
	if(rc == 0)
	{
		// ��ʱ˵���Է��ر��˸�����ͨ���Ķ�ȡ������shutdown(READ)��
		// �յ���FIN����ʱ����EOF
		evt->status = STEAK_EVENT_STATUS_DELETE;
		return STEAK_ERR_OK;
	}
	else if(rc == -1)
	{
		// socket��������, ��event���Ϊ��Ч������ɾ��event
		evt->status = STEAK_EVENT_STATUS_DELETE;
		return STEAK_ERR_OK;
	}
	else
	{
		// ������һЩ�ֽ���
		// ����http����
		parser->readsize = data_size;
		steak_parser_parse(parser);

		if(parser->completed)
		{
			YLOGI("http request receive completed");
		}
	}

	return STEAK_ERR_OK;
}
