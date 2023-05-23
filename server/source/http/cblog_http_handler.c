#include <stdio.h>
#include <stdio.h>
#include <string.h>

#include "cblog_errors.h"
#include "cblog_types.h"
#include "cblog_buffer.h"
#include "cblog_string.h"
#include "cblog_response.h"

static void process_request(cblog_http_context *ctx)
{
	svchost *svc = ctx->svc;
	cblog_request *request = ctx->request;
	cblog_response *response = ctx->response;

	response->version = request->version;

	switch(request->method)
	{
		case STEAK_HTTP_METHOD_GET:
		{
			if(!cblog_string_casecmp(&request->url, "/") ||
				!cblog_string_casecmp(&request->url, svc->options.index_page))
			{
				// 访问的是主页
				response->status_code = CBLOG_HTTP_STATUS_CODE_OK;
				cblog_buffer_write(response->body_buffer, CBLOG_WELCOME_MESSAGE, strlen(CBLOG_WELCOME_MESSAGE));
				break;
			}

			// 获取资源完整路径
			char url[64] = { '\0' };
			cblog_string_copy(&request->url, url, sizeof(url));
			char path[512] = { '\0' };
			snprintf(path, sizeof(path), "%s%s", svc->options.root, url);

			// 判断资源是否存在
			int exist = Y_file_exist(path);
			if(exist)
			{
				// 资源不存在，返回404
				response->status_code = CBLOG_HTTP_STATUS_NOT_FOUND;
				break;
			}

			// 资源存在，读取资源并保存到body里
			int rc = cblog_buffer_write_file(response->body_buffer, path);
			if(rc != CBLOG_ERR_OK)
			{
				// 读文件失败，返回500
				response->status_code = CBLOG_HTTP_STATUS_INTERNAL_SERVER_ERROR;
				break;
			}

			// 读文件成功，返回200
			response->status_code = CBLOG_HTTP_STATUS_CODE_OK;

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