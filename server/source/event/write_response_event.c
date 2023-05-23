#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>

#include <libY.h>

#include "cblog_errors.h"
#include "cblog_app.h"
#include "cblog_event_module.h"
#include "cblog_factory.h"
#include "cblog_buffer.h"
#include "cblog_string.h"

/// <summary>
/// 2023-5-18 20:17
/// ���Է�����Ӧ��HTTP�ͻ�����
/// </summary>
/// <param name="conn"></param>
static void send_hello_cblog(cblog_connection *conn)
{
	char *str = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nhello cblog!";
	send(conn->sock, str, strlen(str), 0);
}

static void dump_response(cblog_pending_response *response)
{
	cblog_string str =
	{
		.buffer = response->buffer,
		.length = response->buffer->left,
		.offset = 0
	};
	cblog_string_print(&str);
}



int write_response_event(event_module *evm, cblog_event *evt)
{
	cblog_connection *conn = (cblog_connection *)evt->context;

	// ��ǰҪд�����Ӧ
	cblog_pending_response_chain *response_chain = conn->pending_response_list;

	// �����Ƿ�����쳣������쳣����ô��Ѹ��¼����뵽except_events�Ȼ��ر�����
	int except = 0;

#pragma region �벻Ҫɾ�����д��룬���д�����cblog���͵ĵ�һ��HTTP��Ӧ
	//send_hello_cblog(conn);

	// �ļ���������Ϊ����д
	//event_modify(evm, evt, evt->read, 0);
#pragma endregion

	// ������׽���Ϊ������ģʽ
	// ����һ��������TCP�׽��֣�����䷢�ͻ�������û�пռ䣬���̽�����������״̬��ֱ���пռ�λ�á�
	// ����һ����������TCP�׽��֣�����䷢�ͻ������и���û�пռ䣬send�������ý���������һ��EWOULDBLOCK����
	// ����䷢�ͻ���������һЩ�ռ䣬����ֵ�����ں��ܹ����Ƶ��û������е��ֽ���������ֽ���Ҳ��Ϊ���������short count����

	// ���жϴ����Ͷ����Ƿ�Ϊ�գ���Ϊ�յĻ������ȷ��ʹ����Ͷ����������
	cblog_pending_response *response = conn->pending_response_list->first;
	dump_response(response);
	while(response != NULL)
	{
		cblog_pending_response *current = response;
		response = response->next;

		int left = current->left;
		int offset = current->offset;
		int rc = send(conn->sock, current->buffer->pdata + offset, left, 0);
		if(rc < 0)
		{
			// ��������
			int sockerr = cblog_socket_error();
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
			if(sockerr == WSAEWOULDBLOCK)
#elif (defined(ENV_UNIX))
			if(sockerr == EWOULDBLOCK)
#endif
			{
				// ��ʱ˵������ϵͳsocket���ͻ�������ռ䲻��
				// ��Ҫ�´���ѯ�ټ�������
			}
			else
			{
				// ��ʱ˵��socket�����쳣������ر�����
				YLOGE("send error, socket_error = %d", sockerr);
				except = 1;
			}
			break;
		}

		if(rc > 0)
		{
			// ������һЩ����
			if(rc == left)
			{
				// ���ݶ��������ˣ��ӷ��Ͷ�����ɾ����Ȼ�����������һ����ֱ�����ͷ���EWOULDBLOCK��������û������Ϊֹ
				Y_chain_remove(cblog_pending_response, conn->pending_response_list, current);
				free_cblog_pending_response(current);
				continue;
			}
			else
			{
				// ����û������
				current->left -= rc;
				current->offset += rc;
				break;
			}
		}

		if(rc == 0)
		{
			// ��ʱ��֪��ʲôʱ��ᷢ��..
			YLOGE("send return 0");
		}
	}

	// ��������Ӧ������Ҫkeep-alive
	// ֱ�ӹر�����
	if(!conn->keep_alive || except)
	{
		Y_queue_enqueue(evm->except_events, evt);
	}
	else
	{
		if(conn->pending_response_list->count == 0)
		{
			// �������ݶ�д���ˣ���ô�ر�д�¼�
			YLOGD("response send completed, disable write event");
			event_modify(evm, evt, evt->read, 0);
		}
	}

	return CBLOG_ERR_OK;
}
