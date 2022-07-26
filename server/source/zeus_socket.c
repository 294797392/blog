#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef ZEUS_WIN32
#include <Windows.h>
#include <WinSock2.h>
#endif

#include "zeus_log.h"
#include "zeus_os.h"
#include "zeus.h"
#include "zeus_socket.h"
#include "zeus_errno.h"

extern struct zeus_socket_ops_s socket_ops_select;
static struct zeus_socket_ops_s *socket_operations[] = 
{
    &socket_ops_select,
};

int new_zeus_socket(zeus_socket **zsocket, ZEUS_SOCKET_MODE mode)
{
    zeus_socket *sock = (zeus_socket*)zeus_calloc(1, sizeof(zeus_socket));

    // 根据mode找到对应的poll
    int num_polls = sizeof(socket_operations) / sizeof(struct zeus_socket_ops_s);
    for(int i = 0; i < num_polls; i++)
    {
        zeus_socket_ops *ops = socket_operations[i];
        if(ops->mode == mode)
        {
            sock->operations = ops;
            break;
        }
    }
    if(sock->operations == NULL)
    {
        zloge(ZTEXT("没找到对应的poll_ops, mode = %d"), mode);
        return ZERR_INVALID_PARAM;
    }

    // 初始化poll的内部对象
    sock->private = sock->operations->initialize(sock);
    if(p->private == NULL)
    {
        return ZERR_FAIL;
    }

    // 创建Socket
    sock->skfd = sock->operations->create_socket(sock);
    if(sock->skfd > 0)
    {
        zloge(ZTEXT("创建socket失败, mode = %d"), mode);
        return ZERR_INVALID_PARAM;
    }

    *zsocket = sock;

    return ZERR_OK;
}

int delete_zeus_socket(zeus_socket *zsocket)
{
    free(zsocket);
}

void zeus_socket_new_data_callback(zeus_socket *zsocket, zeus_svc_new_data_func callback, void *userdata)
{
    zsocket->cb_new_data = callback;
    zsocket->userdata = userdata;
}
