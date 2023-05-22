#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>

#include <libY.h>

#include "cblog_errors.h"
#include "cblog_app.h"
#include "cblog_event_module.h"
#include "cblog_factory.h"


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

static char *new_buffer_from_response(cblog_response *response)
{
	const char *status_text = cblog_http_status_code_string(response->status_code);
	const char *version = cblog_http_version_string(response->version);

	char status_line[512] = { '\0' };
	snprintf(status_line, sizeof(status_line), "%s %d %s\r\n", version, response->status_code, status_text);

	// HTTP�����ܳ���
	//int len = strlen(status_line) + response->header_len + response->body_len + 2;

	//char *buffer = (char *)calloc(1, sizeof(len));
	//strncpy(buffer, status_line, strlen(status_line));
}



int write_response_event(event_module *evm, cblog_event *evt)
{
	cblog_connection *conn = (cblog_connection *)evt->context;

	// ��ǰҪд�����Ӧ
	cblog_response *response = conn->response;

#pragma region �벻Ҫɾ�����д��룬���д�����cblog���͵ĵ�һ��HTTP��Ӧ
	send_hello_cblog(conn);

	// �ļ���������Ϊ����д
	event_modify(evm, evt, evt->read, 0);
#pragma endregion


	// ���жϴ����Ͷ����Ƿ�Ϊ�գ���Ϊ�յĻ������ȷ��ʹ����Ͷ����������



	// ���Է�����Ӧ������Ӧû����ȫ�����꣬��ô���뵽pending_request����
	//cblog_pending_response *pending_response = new_cblog_pending_response(response);
	//Y_linklist_add(cblog_pending_response, (&conn->pending_response_list), pending_response);

	// ������׽���Ϊ������ģʽ
	// ����һ��������TCP�׽��֣�����䷢�ͻ�������û�пռ䣬���̽�����������״̬��ֱ���пռ�λ�á�
	// ����һ����������TCP�׽��֣�����䷢�ͻ������и���û�пռ䣬send�������ý���������һ��EWOULDBLOCK����
	// ����䷢�ͻ���������һЩ�ռ䣬����ֵ�����ں��ܹ����Ƶ��û������е��ֽ���������ֽ���Ҳ��Ϊ���������short count����
	//send(conn->sock, response->)

	// ��������Ӧ������Ҫkeep-alive
	// ֱ�ӹر�����
	if(!conn->keep_alive)
	{
		Y_queue_enqueue(evm->except_events, evt);
	}

	return STEAK_ERR_OK;
}
