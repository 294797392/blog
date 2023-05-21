#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "cblog_default.h"
#include "cblog_errors.h"
#include "cblog_types.h"
#include "cblog_parser.h"
#include "cblog_event_module.h"

extern void http_parser_event_handler(steak_parser *parser, steak_parser_event_enum evt);
extern int read_request_event(event_module *evm, cblog_event *evt);
extern int write_response_event(event_module *evm, cblog_event *evt);
extern int accept_client_event(event_module *evm, cblog_event *evt);

int event_add(event_module *evm, cblog_event *evt)
{
	eventpoll_actions *actions = evm->actions;
	actions->add_event(evm, evt);
	Y_linklist_add(cblog_event, evm->events, evt);
	return STEAK_ERR_OK;
}

int event_remove(event_module *evm, cblog_event *evt)
{
	eventpoll_actions *actions = evm->actions;
	actions->delete_event(evm, evt);
	Y_linklist_remove(cblog_event, evm->events, evt);
	return STEAK_ERR_OK;
}

int event_modify(event_module *evm, cblog_event *evt, int read, int write)
{
	eventpoll_actions *actions = evm->actions;

	if(evt->read == read && evt->write == write)
	{
		return STEAK_ERR_OK;
	}

	evt->read = read;
	evt->write = write;

	return actions->modify_event(evm, evt, read, write);
}

int event_run_cycle(event_module *evm)
{
	int code = STEAK_ERR_OK;
	eventpoll_actions *actions = evm->actions;
	code = actions->poll_event(evm);

	// 删除连接出现异常情况的事件
	cblog_event *evt = Y_queue_dequeue(evm->except_events);
	while(evt != NULL)
	{
		cblog_connection *conn = (cblog_connection *)evt->context;
		YLOGI("close connection, reason = %d", conn->status);
		steak_socket_close(evt->sock);
		event_remove(evm, evt);
		free_connection_event(evm, evt);
		evt = Y_queue_dequeue(evm->except_events);
	}

	return code;
}

cblog_event *new_connection_event(event_module *evm, cblog_socket sock, svchost *svc)
{
	// 创建connection实例
	cblog_connection *conn = (cblog_connection *)calloc(1, sizeof(cblog_connection));
	conn->sock = sock;
	conn->svc = svc;
	conn->status = CBLOG_CONN_STATUS_CONNECTED;
	conn->keep_alive = 1; // 默认开始keep-alive
	conn->recvbuf = new_cblog_sockbuf(sock, CBLOG_DEFAULT_RECV_BUF_SIZE);

	cblog_request *request = (cblog_request *)calloc(1, sizeof(cblog_request));
	request->max_header = STEAK_DEFAULT_HEADER_COUNT;
	conn->request = request;

	cblog_response *response = (cblog_response *)calloc(1, sizeof(cblog_response));
	conn->response = response;

	// 配置选项参数
	cblog_connection_options *opts = &conn->options;
	opts->save_request = 1;
	//opts->save_request_path = "request.txt";

	// 配置HTTP解析器参数
	steak_parser *parser = &conn->parser;
	parser->state = STEAK_PARSER_INITIAL;
	parser->userdata = conn;
	parser->on_event = http_parser_event_handler;

	// 创建event实例
	cblog_event *evt = (cblog_event *)calloc(1, sizeof(cblog_event));
	evt->read = 1;
	evt->write = 0;
	evt->on_read = read_request_event;
	evt->on_write = write_response_event;
	evt->context = conn;
	evt->type = STEAK_EVENT_TYPE_CONNECTION;
	evt->sock = sock;
	return evt;
}

void free_connection_event(event_module *evm, cblog_event *evt)
{
	// 释放connection
	cblog_connection *conn = (cblog_connection *)evt->context;
	free_cblog_sockbuf(conn->recvbuf);
	free(conn->request);
	free(conn);

	// 释放event
	free(evt);
}

cblog_event *new_svchost_event(event_module *evm, svchost *svc)
{
	cblog_event *evt = (cblog_event *)calloc(1, sizeof(cblog_event));
	evt->read = 1;
	evt->on_read = accept_client_event;
	evt->context = svc;
	evt->type = STEAK_EVENT_TYPE_SVCHOST;
	evt->sock = svc->sock;
	return evt;
}
