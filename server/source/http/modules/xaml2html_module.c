#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cblog_errors.h"
#include "cblog_http_module.h"

typedef struct xaml2html_s
{
	void *ptr;
}xaml2html;

static int on_cblog_begin_request_http_event(cblog_http_context *http_context)
{
	return CBLOG_ERR_OK;
}

static int on_cblog_end_request_http_event(cblog_http_context *http_context)
{
	return CBLOG_ERR_OK;
}

static int on_cblog_begin_response_http_event(cblog_http_context *http_context)
{
	return CBLOG_ERR_OK;
}

static int on_cblog_end_response_http_event(cblog_http_context *http_context)
{
	return CBLOG_ERR_OK;
}



static void *xaml2html_initialize(cblog_app *app)
{
	xaml2html *x2h = (xaml2html *)calloc(1, sizeof(xaml2html));

	cblog_http_add_event(app->http_evlist, CBLOG_HTTP_EVENT_BEGIN_REUQEST, on_cblog_begin_request_http_event);
	cblog_http_add_event(app->http_evlist, CBLOG_HTTP_EVENT_END_REQUEST, on_cblog_end_request_http_event);
	cblog_http_add_event(app->http_evlist, CBLOG_HTTP_EVENT_BEGIN_RESPONSE, on_cblog_begin_response_http_event);
	cblog_http_add_event(app->http_evlist, CBLOG_HTTP_EVENT_END_RESPONSE, on_cblog_end_response_http_event);

	return x2h;
}

static void xaml2html_release(void *ctx)
{
	xaml2html *x2h = (xaml2html *)ctx;
	free(x2h);
}


cblog_http_module xaml2html_module = 
{
	.name = "demo http module",
	.desc = "demonstrate how to extend http module. follow ASP.NET's HttpModule",
	.author = "oheiheiheiheihei",
	.initialize = xaml2html_initialize,
	.release = xaml2html_release,
};
