#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>

#include <libY.h>

#include "errors.h"
#include "event.h"
#include "app.h"

int read_request_event(event_module *evm, steak_event *evt)
{
	steak_session *session = (steak_session *)evt->context;
	steak_request *request = session->request;

	char *recv_buf = request->buffer + request->write_offset;
	int recv_buf_size = request->buffer_size - request->write_offset;

	int rc = recv(evt->sock, recv_buf, recv_buf_size, 0);
	if(rc == 0)
	{
		// ��ʱ˵���Է��ر��˸�����ͨ���Ķ�ȡ������shutdown(READ)��
		// �յ���FIN����ʱ����EOF
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

		// �����������ȡλ�ôﵽ�˻�����ĩβ����ô������
		if(request->write_offset == request->buffer_size)
		{
			int newsize = request->buffer_size * 2;
			char *buffer = (char *)realloc(request->buffer, newsize);
			if(buffer == NULL)
			{
				YLOGE("realloc request buffer failed, %s", strerror(errno));
				return STEAK_ERR_NO_MEM;
			}
			request->buffer = buffer;
			request->buffer_size = newsize;
		}

		request->write_offset += rc;
	}

	// ����http����
	//steak_parser_parse(request);

	return STEAK_ERR_FAILED;
}