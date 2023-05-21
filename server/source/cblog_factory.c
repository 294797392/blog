#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cblog_factory.h"

cblog_request *new_cblog_request()
{
	cblog_request *request = (cblog_request *)calloc(1, sizeof(cblog_request));
	request->nheader = 0;
	request->max_header = STEAK_DEFAULT_HEADER_COUNT;
	return request;
}

void free_cblog_request(cblog_request *request)
{
	free(request);
}

cblog_response *new_cblog_response()
{
	cblog_response *response = (cblog_response *)calloc(1, sizeof(cblog_response));
	response->nheader = 0;
	response->max_header = STEAK_DEFAULT_HEADER_COUNT;
	return response;
}

void free_cblog_response(cblog_response *response)
{
	free(response);
}

cblog_pending_response *new_cblog_pending_response(cblog_response *response)
{
	cblog_pending_response *pending_response = (cblog_pending_response *)calloc(1, sizeof(cblog_pending_response));
	return pending_response;
}

cblog_pending_response *new_cblog_pending_response2(char *buf, int buflen)
{
	cblog_pending_response *pending_response = (cblog_pending_response *)calloc(1, sizeof(cblog_pending_response));
	pending_response->buffer = buf;
	pending_response->buflen = buflen;
	return pending_response;
}

void free_cblog_pending_response(cblog_pending_response *response)
{
	free(response->buffer);
	free(response);
}

extern void http_parser_event_handler(cblog_parser *parser, cblog_parser_event_enum evt);
cblog_connection *new_cblog_connection(cblog_socket sock, svchost *svc)
{
	// 创建connection实例
	cblog_connection *conn = (cblog_connection *)calloc(1, sizeof(cblog_connection));
	conn->sock = sock;
	conn->svc = svc;
	conn->status = CBLOG_CONN_STATUS_CONNECTED;
	conn->keep_alive = 1; // 默认开始keep-alive
	conn->recvbuf = new_cblog_sockbuf(sock, CBLOG_DEFAULT_RECV_BUF_SIZE);
	conn->request = new_cblog_request();
	conn->response = new_cblog_response();

	// 配置选项参数
	cblog_connection_options *opts = &conn->options;
	opts->save_request = 1;
	//opts->save_request_path = "request.txt";

	// 配置HTTP解析器参数
	cblog_parser *parser = &conn->parser;
	parser->state = CBLOG_PARSER_STATE_INITIAL;
	parser->userdata = conn;
	parser->on_event = http_parser_event_handler;

	return conn;
}

void free_cblog_connection(cblog_connection *conn)
{
	// 释放pending_response_chain
	cblog_pending_response_chain *chain = &conn->pending_response_list;
	cblog_pending_response *pending_response = chain->first;
	while(pending_response != NULL)
	{
		free_cblog_pending_response(pending_response);
		pending_response = pending_response->next;
	}
	Y_linklist_clear(chain);

	free_cblog_response(conn->response);
	free_cblog_request(conn->request);
	free_cblog_sockbuf(conn->recvbuf);
	free(conn);
}

extern int read_request_event(event_module *evm, cblog_event *evt);
extern int write_response_event(event_module *evm, cblog_event *evt);
cblog_event *new_cblog_connection_event(cblog_socket sock, svchost *svc)
{
	// 创建event实例
	cblog_event *evt = (cblog_event *)calloc(1, sizeof(cblog_event));
	evt->read = 1;
	evt->write = 0;
	evt->on_read = read_request_event;
	evt->on_write = write_response_event;
	evt->context = new_cblog_connection(sock, svc);
	evt->type = CBLOG_EVENT_TYPE_CONNECTION;
	evt->sock = sock;
	return evt;
}

void free_cblog_connection_event(cblog_event *evt)
{
	cblog_connection *conn = (cblog_connection *)evt->context;
	free_cblog_connection(conn);
	free(evt);
}

extern int accept_client_event(event_module *evm, cblog_event *evt);
cblog_event *new_cblog_svchost_event(svchost *svc)
{
	cblog_event *evt = (cblog_event *)calloc(1, sizeof(cblog_event));
	evt->read = 1;
	evt->on_read = accept_client_event;
	evt->context = svc;
	evt->type = CBLOG_EVENT_TYPE_SVCHOST;
	evt->sock = svc->sock;
	return evt;
}

void free_cblog_svchost_event(cblog_event *evt)
{
	free(evt);
}
