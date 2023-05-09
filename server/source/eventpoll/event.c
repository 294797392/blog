#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "errors.h"
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


event_module *new_eventpoll(event_module_options *options)
{
	event_module *evpoll = (event_module *)calloc(1, sizeof(event_module));
	evpoll->options = options;
	evpoll->event_list = Y_create_list();
	evpoll->actions = select_evpoll_actions(options);
	evpoll->actions->initialize(evpoll);
	return evpoll;
}

void free_eventpoll(event_module *evpoll)
{
	free(evpoll);
}

int event_add(event_module *evpoll, steak_event *evt)
{
	eventpoll_actions *actions = evpoll->actions;
	actions->add_event(evpoll, evt);

	Y_list_add(evpoll->event_list, evt);

	return STEAK_ERR_OK;
}

int event_delete(event_module *evpoll, steak_event *evt)
{
	eventpoll_actions *actions = evpoll->actions;
	actions->delete_event(evpoll, evt);

	Y_list_remove(evpoll->event_list, evt, 0);

	return STEAK_ERR_OK;
}

int event_poll(event_module *evpoll)
{
	int code = STEAK_ERR_OK;
	eventpoll_actions *actions = evpoll->actions;
	code = actions->poll_event(evpoll);

	return code;
}

steak_event *new_event(event_module *evpoll)
{
	steak_event *evt = (steak_event *)calloc(1, sizeof(steak_event));
	return evt;
}

void free_event(event_module *evpoll, steak_event *evt)
{
	free(evt);
}

