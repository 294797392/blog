#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "cblog_default.h"
#include "cblog_errors.h"
#include "cblog_types.h"
#include "cblog_parser.h"
#include "cblog_event_module.h"
#include "cblog_factory.h"

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
		cblog_socket_close(evt->sock);
		event_remove(evm, evt);
		free_cblog_connection_event(evt);
		evt = Y_queue_dequeue(evm->except_events);
	}

	return code;
}

