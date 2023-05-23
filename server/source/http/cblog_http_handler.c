#include <stdio.h>
#include <stdio.h>
#include <string.h>

#include "cblog_types.h"
#include "cblog_buffer.h"
#include "cblog_string.h"
#include "cblog_response.h"

static void process_request(cblog_http_context *ctx)
{
	svchost *svc = ctx->svc;
	cblog_request *request = ctx->request;
	cblog_response *response = ctx->response;

	switch(request->method)
	{
		case STEAK_HTTP_METHOD_GET:
		{
			if(!cblog_string_casecmp(&request->url, "/") ||
				!cblog_string_casecmp(&request->url, svc->options.index_page))
			{
				// 访问的是主页
				response->version = request->version;
				response->status_code = CBLOG_HTTP_STATUS_CODE_OK;
				cblog_buffer_write(response->body_buffer, CBLOG_WELCOME_MESSAGE, strlen(CBLOG_WELCOME_MESSAGE));
			}

			break;
		}

		default:
			break;
	}
}

struct cblog_http_handler_s cblog_default_http_handler =
{
	.process_request = process_request
};