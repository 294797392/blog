#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>

#include "default.h"
#include "session.h"
#include "request.h"
#include "response.h"
#include "connection.h"
#include "parser.h"
#include "factory.h"

steak_connection *new_connection(steak_socket sock)
{
	steak_connection *conn = (steak_connection *)Y_pool_obtain(sizeof(steak_connection));
	conn->sock = sock;

	steak_parser *parser = &conn->parser;
	parser->state = STEAK_PARSER_INITIAL;
	parser->raw_msg = (char *)Y_pool_obtain(STEAK_DEFAULT_HTTP_MSG_SIZE);
	parser->raw_msg_len = STEAK_DEFAULT_HTTP_MSG_SIZE;
	parser->raw_msg_offset = 0;

	return conn;
}

void free_connection(steak_connection *conn)
{
	Y_pool_recycle(conn->parser.raw_msg, conn->parser.raw_msg_len);
	Y_pool_recycle(conn, sizeof(steak_connection));
}

steak_session *new_session(steak_socket sock)
{
	steak_session *session = (steak_session *)Y_pool_obtain(sizeof(steak_session));
	return session;
}

void free_session(steak_session *session)
{
	Y_pool_recycle(session, sizeof(steak_session));
}
