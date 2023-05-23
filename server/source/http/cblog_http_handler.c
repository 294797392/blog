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
				// ���ʵ�����ҳ
				response->status_code = CBLOG_HTTP_STATUS_CODE_OK;
				cblog_buffer_write(response->body_buffer, CBLOG_WELCOME_MESSAGE, strlen(CBLOG_WELCOME_MESSAGE));
				break;
			}

			// ��ȡ��Դ����·��
			char url[64] = { '\0' };
			cblog_string_copy(&request->url, url, sizeof(url));
			char path[512] = { '\0' };
			snprintf(path, sizeof(path), "%s%s", svc->options.root, url);

			// �ж���Դ�Ƿ����
			int exist = Y_file_exist(path);
			if(exist)
			{
				// ��Դ�����ڣ�����404
				response->status_code = CBLOG_HTTP_STATUS_NOT_FOUND;
				break;
			}

			// ��Դ���ڣ���ȡ��Դ�����浽body��
			int rc = cblog_buffer_write_file(response->body_buffer, path);
			if(rc != CBLOG_ERR_OK)
			{
				// ���ļ�ʧ�ܣ�����500
				response->status_code = CBLOG_HTTP_STATUS_INTERNAL_SERVER_ERROR;
				break;
			}

			// ���ļ��ɹ�������200
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