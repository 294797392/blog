#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "errors.h"
#include "eventpoll.h"

extern eventpoll_actions eventpoll_actions_select;
eventpoll_actions *eventpoll_actions_list[] =
{
	&eventpoll_actions_select
};

static eventpoll_actions *select_evpoll_actions(eventpoll_options *options)
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


eventpoll *new_eventpoll(eventpoll_options *options)
{
	eventpoll *evpoll = (eventpoll *)calloc(1, sizeof(eventpoll));
	evpoll->options = options;
	evpoll->event_list = Y_create_list();
	evpoll->actions = select_evpoll_actions(options);
	evpoll->actions->initialize(evpoll);
	return evpoll;
}

void free_eventpoll(eventpoll *evpoll)
{
	free(evpoll);
}

int eventpoll_add_event(eventpoll *evpoll, eventpoll_event *evt)
{
	eventpoll_actions *actions = evpoll->actions;
	actions->add_event(evpoll, evt);

	Y_list_add(evpoll->event_list, evt);

	return ERR_SUCCESS;
}

int eventpoll_delete_event(eventpoll *evpoll, eventpoll_event *evt)
{
	eventpoll_actions *actions = evpoll->actions;
	actions->delete_event(evpoll, evt);

	Y_list_remove(evpoll->event_list, evt, 0);

	return ERR_SUCCESS;
}

int eventpoll_poll(eventpoll *evpoll)
{
	int code = ERR_SUCCESS;
	eventpoll_actions *actions = evpoll->actions;
	code = actions->poll_event(evpoll);

	return code;
}

eventpoll_event *new_event(eventpoll *evpoll)
{
	eventpoll_event *evt = (eventpoll_event *)calloc(1, sizeof(eventpoll_event));
	return evt;
}

void free_event(eventpoll *evpoll, eventpoll_event *evt)
{
	free(evt);
}

