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
/// 可以发送响应给HTTP客户端了
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

	// HTTP报文总长度
	//int len = strlen(status_line) + response->header_len + response->body_len + 2;

	//char *buffer = (char *)calloc(1, sizeof(len));
	//strncpy(buffer, status_line, strlen(status_line));
}



int write_response_event(event_module *evm, cblog_event *evt)
{
	cblog_connection *conn = (cblog_connection *)evt->context;

	// 当前要写入的响应
	cblog_response *response = conn->response;

#pragma region 请不要删除这行代码，这行代码是cblog发送的第一个HTTP响应
	send_hello_cblog(conn);

	// 文件描述符改为不可写
	event_modify(evm, evt, evt->read, 0);
#pragma endregion


	// 先判断待发送队列是否为空，不为空的话，优先发送待发送队列里的数据



	// 尝试发送响应，若响应没有完全发送完，那么加入到pending_request队列
	//cblog_pending_response *pending_response = new_cblog_pending_response(response);
	//Y_linklist_add(cblog_pending_response, (&conn->pending_response_list), pending_response);

	// 这里的套接字为非阻塞模式
	// 对于一个阻塞的TCP套接字，如果其发送缓冲区中没有空间，进程将被置于休眠状态，直到有空间位置。
	// 对于一个非阻塞的TCP套接字，如果其发送缓冲区中根本没有空间，send函数调用将立即返回一个EWOULDBLOCK错误。
	// 如果其发送缓冲区中有一些空间，返回值将是内核能够复制到该缓冲区中的字节数。这个字节数也称为不足计数（short count）。
	//send(conn->sock, response->)

	// 发送完响应，不需要keep-alive
	// 直接关闭连接
	if(!conn->keep_alive)
	{
		Y_queue_enqueue(evm->except_events, evt);
	}

	return STEAK_ERR_OK;
}
