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

steak_session *new_session(steak_socket sock)
{
	steak_session *session = (steak_session *)Y_pool_obtain(sizeof(steak_session));
	session->request = new_request();
	session->response = new_response();
	session->parser = new_parser();
	session->parser->raw_msg = session->request->raw_msg;
	session->parser->raw_msg_len = session->request->raw_msg_len;
	session->sock = sock;
	return session;
}

void free_session(steak_session *session)
{
	free_request(session->request);
	free_response(session->response);
	free_parser(session->parser);
	Y_pool_recycle(session, sizeof(steak_session));
}

steak_request *new_request()
{
	steak_request *request = (steak_request *)Y_pool_obtain(sizeof(steak_request));
	request->raw_msg = (char *)Y_pool_obtain(STEAK_DEFAULT_RECV_BUF_SIZE);
	request->raw_msg_len = STEAK_DEFAULT_RECV_BUF_SIZE;
	return request;
}

void free_request(steak_request *request)
{
	Y_pool_recycle(request->raw_msg, request->raw_msg_len);
	Y_pool_recycle(request, sizeof(steak_request));
}

steak_response *new_response()
{
	steak_response *response = (steak_response *)Y_pool_obtain(sizeof(steak_response));
	response->raw_msg = (char *)Y_pool_obtain(STEAK_DEFAULT_SEND_BUF_SIZE);
	response->raw_msg_len = STEAK_DEFAULT_SEND_BUF_SIZE;
	return response;
}

void free_response(steak_response *response)
{
	Y_pool_recycle(response, sizeof(steak_response));
}

steak_parser *new_parser()
{
	steak_parser *parser = (steak_parser *)Y_pool_obtain(sizeof(steak_parser));
	parser->state = STEAK_PARSER_INITIAL;
	parser->first_header = (steak_http_header *)Y_pool_obtain(sizeof(steak_http_header));
	parser->last_header = parser->first_header;
	return parser;
}

void free_parser(steak_parser *parser)
{
	Y_pool_recycle(parser->first_header, sizeof(steak_http_header));
	Y_pool_recycle(parser, sizeof(steak_parser));
}
