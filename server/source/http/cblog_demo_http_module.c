#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cblog_errors.h"
#include "cblog_http_module.h"
#include "cblog_event_module.h"
#include "cblog_response.h"

typedef struct demo_module_s
{
	void *ptr;
}demo_module;

static int on_cblog_begin_request_http_event(cblog_http_context *http_context)
{
	YLOGI("on_cblog_begin_request_http_event");

	return CBLOG_ERR_OK;
}

static int on_cblog_end_request_http_event(cblog_http_context *http_context)
{
	YLOGI("on_cblog_end_request_http_event");

	return CBLOG_ERR_OK;
}

static int on_cblog_begin_response_http_event(cblog_http_context *http_context)
{
	cblog_response_write_body(http_context->response, "on_cblog_begin_response_http_event\r\n", strlen("on_cblog_begin_response_http_event\r\n"));
	return CBLOG_ERR_OK;
}

static int on_cblog_end_response_http_event(cblog_http_context *http_context)
{
	cblog_response_write_body(http_context->response, "\r\non_cblog_end_response_http_event\r\n", strlen("\r\non_cblog_end_response_http_event\r\n"));
	return CBLOG_ERR_OK;
}



static void *cblog_demo_http_module_initialize(cblog_app *app)
{
	demo_module *demo = (demo_module *)calloc(1, sizeof(demo_module));

	cblog_http_add_event(app->http_evlist, CBLOG_HTTP_EVENT_BEGIN_REUQEST, on_cblog_begin_request_http_event);
	cblog_http_add_event(app->http_evlist, CBLOG_HTTP_EVENT_END_REQUEST, on_cblog_end_request_http_event);
	cblog_http_add_event(app->http_evlist, CBLOG_HTTP_EVENT_BEGIN_RESPONSE, on_cblog_begin_response_http_event);
	cblog_http_add_event(app->http_evlist, CBLOG_HTTP_EVENT_END_RESPONSE, on_cblog_end_response_http_event);

	return demo;
}

static void cblog_demo_http_module_release(void *ctx)
{
	demo_module *demo = (demo_module *)ctx;
	free(demo);
}

cblog_http_module cblog_demo_http_module =
{
	.name = "demo http module",
	.desc = "demonstrate how to extend http module. follow ASP.NET's HttpModule",
	.author = "oheiheiheiheihei",
	.initialize = cblog_demo_http_module_initialize,
	.release = cblog_demo_http_module_release,
};

