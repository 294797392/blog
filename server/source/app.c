#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <libY.h>

#include "default.h"
#include "errors.h"
#include "app.h"

steak_app *app = NULL;

int steak_app_init()
{
	app = (steak_app *)calloc(1, sizeof(steak_app));
	return STEAK_ERR_OK;
}

steak_app *get_steak_app()
{
	return app;
}

steak_session *new_session()
{
	steak_session *session = (steak_session *)calloc(1, sizeof(steak_session));
	session->request = new_request();
	session->response = new_response();
	return session;
}

void free_session(steak_session *session)
{
	free(session);
}

steak_request *new_request()
{
	steak_request *request = (steak_request *)calloc(1, sizeof(steak_request));
	request->buffer = (char *)calloc(1, STEAK_DEFAULT_RECV_BUF_SIZE);
	request->buffer_size = STEAK_DEFAULT_RECV_BUF_SIZE;
	return request;
}

void free_request(steak_request *request)
{
	free(request);
}

steak_response *new_response() 
{
	steak_response *response = (steak_response *)calloc(1, sizeof(steak_response));
	response->buffer = (char *)calloc(1, STEAK_DEFAULT_SEND_BUF_SIZE);
	response->buffer_size = STEAK_DEFAULT_SEND_BUF_SIZE;
	return response;
}

void free_response(steak_response *response)
{
	free(response);
}
