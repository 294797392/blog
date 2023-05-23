#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "cblog_http_event.h"

void cblog_http_add_event(cblog_http_event_list *evlist, cblog_http_event_enum evtype, cblog_http_event_handler handler)
{
	cblog_http_event_chain *chain = &evlist->chain_list[evtype];
	cblog_http_event *evt = (cblog_http_event *)calloc(1, sizeof(cblog_http_event));
	evt->handler = handler;
	Y_chain_add(cblog_http_event, chain, evt);
}

void cblog_http_remove_event(cblog_http_event_list *evlist, cblog_http_event_enum evtype, cblog_http_event_handler handler)
{}

void cblog_http_raise_event(cblog_http_event_list *evlist, cblog_http_event_enum evtype, cblog_connection *conn)
{
	cblog_http_event_chain *chain = &evlist->chain_list[evtype];

	cblog_http_context http_context =
	{
		.request = conn->request,
		.response = conn->response,
		.svc = conn->svc
	};

	Y_chain_foreach(cblog_http_event, chain,
		{
			current->handler(&http_context);
		});
}
