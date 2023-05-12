#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "errors.h"
#include "default.h"
#include "event.h"

extern eventpoll_actions eventpoll_actions_select;
eventpoll_actions *eventpoll_actions_list[] =
{
	&eventpoll_actions_select
};



static eventpoll_actions *select_evpoll_actions(event_module_options *options)
{
	size_t len = sizeof(eventpoll_actions_list) / sizeof(eventpoll_actions *);

	for(size_t i = 0; i < len; i++)
	{
		eventpoll_actions *actions = eventpoll_actions_list[i];
		if(actions->type == options->type)
		{
			return actions;
		}
	}

	return NULL;
}

steak_event *new_event(event_module *evm)
{
	steak_event *evt = (steak_event *)Y_pool_obtain(sizeof(steak_event));
	return evt;
}



event_module *new_event_module()
{
	event_module_options *evmops = (event_module_options *)calloc(1, sizeof(event_module_options));
	evmops->type = EVENT_POLL_TYPE_SELECT;

	event_module *evm = (event_module *)calloc(1, sizeof(event_module));
	evm->options = evmops;
	evm->timeout_ms = STEAK_DEFAULT_POLL_TIMEOUT;
	evm->event_list = Y_create_list();
	evm->process_event_list = Y_create_list();
	evm->actions = select_evpoll_actions(evmops);
	evm->actions->initialize(evm);
	return evm;
}

void free_event_module(event_module *evm)
{
	free(evm);
}

int event_add(event_module *evm, steak_event *evt)
{
	eventpoll_actions *actions = evm->actions;
	actions->add_event(evm, evt);

	Y_list_add(evm->event_list, evt);

	return STEAK_ERR_OK;
}

int event_delete(event_module *evm, steak_event *evt)
{
	eventpoll_actions *actions = evm->actions;
	actions->delete_event(evm, evt);

	Y_list_remove(evm->event_list, evt);

	return STEAK_ERR_OK;
}

int event_modify(event_module *evm, steak_event *evt, int read, int write)
{
	eventpoll_actions *actions = evm->actions;
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

steak_event *new_session_event(event_module *evm,
	int(*on_read)(event_module *evpoll, steak_event *evt),
	int(*on_write)(event_module *evpoll, steak_event *evt),
	steak_session *session)
{
	steak_event *evt = new_event(evm);
	evt->readable = 1;
	evt->writeable = 1;
	evt->on_read = on_read;
	evt->on_write = on_write;
	evt->context = session;
	evt->type = STEAK_EVENT_TYPE_SESSION;
	evt->sock = session->sock;
	return evt;
}

steak_event *new_svchost_event(event_module *evm, int(*on_read)(event_module *evpoll, steak_event *evt), svchost *svc)
{
	steak_event *evt = new_event(evm);
	evt->readable = 1;
	evt->on_read = on_read;
	evt->context = svc;
	evt->type = STEAK_EVENT_TYPE_SVCHOST;
	evt->sock = svc->sock;
	return evt;
}

void free_event(event_module *evm, steak_event *evt)
{
	Y_pool_recycle(evt, sizeof(steak_event));
}

