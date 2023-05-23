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
/// 可以发送响应给HTTP客户端了
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

	// 当前要写入的响应
	cblog_pending_response_chain *response_chain = conn->pending_response_list;

	// 发送是否出现异常，如果异常，那么会把该事件加入到except_events里，然后关闭连接
	int except = 0;

#pragma region 请不要删除这行代码，这行代码是cblog发送的第一个HTTP响应
	//send_hello_cblog(conn);

	// 文件描述符改为不可写
	//event_modify(evm, evt, evt->read, 0);
#pragma endregion

	// 这里的套接字为非阻塞模式
	// 对于一个阻塞的TCP套接字，如果其发送缓冲区中没有空间，进程将被置于休眠状态，直到有空间位置。
	// 对于一个非阻塞的TCP套接字，如果其发送缓冲区中根本没有空间，send函数调用将立即返回一个EWOULDBLOCK错误。
	// 如果其发送缓冲区中有一些空间，返回值将是内核能够复制到该缓冲区中的字节数。这个字节数也称为不足计数（short count）。

	// 先判断待发送队列是否为空，不为空的话，优先发送待发送队列里的数据
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
			// 发生错误
			int sockerr = cblog_socket_error();
#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
			if(sockerr == WSAEWOULDBLOCK)
#elif (defined(ENV_UNIX))
			if(sockerr == EWOULDBLOCK)
#endif
			{
				// 此时说明操作系统socket发送缓冲区里空间不足
				// 需要下次轮询再继续发送
			}
			else
			{
				// 此时说明socket发生异常情况，关闭连接
				YLOGE("send error, socket_error = %d", sockerr);
				except = 1;
			}
			break;
		}

		if(rc > 0)
		{
			// 发送了一些数据
			if(rc == left)
			{
				// 数据都发送完了，从发送队列里删除，然后继续发送下一个，直到发送返回EWOULDBLOCK或者数据没发送完为止
				Y_chain_remove(cblog_pending_response, conn->pending_response_list, current);
				free_cblog_pending_response(current);
				continue;
			}
			else
			{
				// 数据没发送完
				current->left -= rc;
				current->offset += rc;
				break;
			}
		}

		if(rc == 0)
		{
			// 暂时不知道什么时候会发生..
			YLOGE("send return 0");
		}
	}

	// 发送完响应，不需要keep-alive
	// 直接关闭连接
	if(!conn->keep_alive || except)
	{
		Y_queue_enqueue(evm->except_events, evt);
	}
	else
	{
		if(conn->pending_response_list->count == 0)
		{
			// 所有数据都写完了，那么关闭写事件
			YLOGD("response send completed, disable write event");
			event_modify(evm, evt, evt->read, 0);
		}
	}

	return CBLOG_ERR_OK;
}
