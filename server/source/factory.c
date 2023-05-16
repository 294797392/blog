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

steak_session *new_session(steak_socket sock)
{
	steak_session *session = (steak_session *)Y_pool_obtain(sizeof(steak_session));
	return session;
}

void free_session(steak_session *session)
{
	Y_pool_recycle(session, sizeof(steak_session));
}
