#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef ZEUS_WIN32
#include <Windows.h>
#include <WinSock2.h>
#endif

#include "zeus.h"
#include "zeus_socket.h"

static SOCKET initialize(zeus *z)
{

}

static void add_client(zeus *z, SOCKET skfd)
{

}

static void remove_client(zeus *z, SOCKET skfd)
{

}

struct zeus_socket_ops_s socket_ops_select = 
{
    .mode = ZSOCK_MODE_SELECT,
    .initialize = initialize,
    .add_client = add_client,
    .remove_client = remove_client
};

