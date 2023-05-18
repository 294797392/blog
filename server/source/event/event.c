#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "errors.h"
#include "default.h"
#include "event.h"
#include "parser.h"

extern void http_parser_event_handler(steak_parser *parser, steak_parser_event_enum evt);
extern int read_request_event(event_module *evm, steak_event *evt);
extern int write_response_event(event_module *evm, steak_event *evt);
extern int accept_client_event(event_module *evm, steak_event *evt);

int event_add(event_module *evm, steak_event *evt)
{
	eventpoll_actions *actions = evm->actions;
	actions->add_event(evm, evt);

	Y_list_add(evm->event_list, evt);

	return STEAK_ERR_OK;
}

int event_remove(event_module *evm, steak_event *evt)
{
	eventpoll_actions *actions = evm->actions;
	actions->delete_event(evm, evt);

	Y_list_remove(evm->event_list, evt);

	return STEAK_ERR_OK;
}

int event_modify(event_module *evm, steak_event *evt, int read, int write)
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

int event_poll(event_module *evm, steak_event **events, int nevent)
{
	int code = STEAK_ERR_OK;
	eventpoll_actions *actions = evm->actions;
	code = actions->poll_event(evm, events, nevent);
	return code;
}

int event_process(event_module *evm, steak_event **events, int nevent)
{
	for(int i = 0; i < nevent; i++)
	{
		steak_event *evt = events[i];
		evt->on_read(evm, evt);
	}
	return STEAK_ERR_OK;
}

steak_event *new_connection_event(event_module *evm, steak_socket sock, svchost *svc)
{
	steak_connection *conn = (steak_connection *)Y_pool_obtain(sizeof(steak_connection));
	conn->sock = sock;
	conn->svc = svc;
	conn->request = (steak_request *)Y_pool_obtain(sizeof(steak_request));
	conn->recv_buf = (char *)Y_pool_obtain(STEAK_DEFAULT_HTTP_MSG_SIZE);
	conn->recv_buf_len = STEAK_DEFAULT_HTTP_MSG_SIZE;
	conn->recv_buf_offset = 0;

	steak_parser *parser = &conn->parser;
	parser->state = STEAK_PARSER_INITIAL;
	parser->userdata = conn;
	parser->on_event = http_parser_event_handler;

	steak_event *evt = (steak_event *)Y_pool_obtain(sizeof(steak_event));
	evt->read = 1;
	evt->write = 1;
	evt->on_read = read_request_event;
	evt->on_write = write_response_event;
	evt->context = conn;
	evt->type = STEAK_EVENT_TYPE_CONNECTION;
	evt->sock = sock;
	return evt;
}

void free_connection_event(event_module *evm, steak_event *evt)
{
	// 释放connection
	steak_connection *conn = (steak_connection *)evt->context;
	Y_pool_recycle(conn->request, sizeof(steak_request));
	Y_pool_recycle(conn->recv_buf, conn->recv_buf_len);
	Y_pool_recycle(conn, sizeof(steak_connection));

	// 释放event
	Y_pool_recycle(evt, sizeof(steak_event));
}

steak_event *new_svchost_event(event_module *evm, svchost *svc)
{
	steak_event *evt = (steak_event *)Y_pool_obtain(sizeof(steak_event));
	evt->read = 1;
	evt->on_read = accept_client_event;
	evt->context = svc;
	evt->type = STEAK_EVENT_TYPE_SVCHOST;
	evt->sock = svc->sock;
	return evt;
}
