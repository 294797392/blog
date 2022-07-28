#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef ZEUS_WIN32
#include <WinSock2.h>
#include <Windows.h>
#endif

#include "zeus_log.h"
#include "zeus_os.h"
#include "zeus_errno.h"

extern struct zeus_socket_s socket_io_select;
static struct zeus_socket_s *socket_io_list[] = 
{
    &socket_io_select,
};

zeus_svc *new_zeus_svc(int port)
{
    zeus_svc *svc = (zeus_svc *)zeus_calloc(1, sizeof(zeus_svc));
    svc->port = port;

    // 根据mode找到对应的IO模型
    int num_io = sizeof(socket_io_list) / sizeof(struct zeus_socket_io_s*);
    for(int i = 0; i < num_io; i++)
    {
        zeus_socket *zs = socket_io_list[i];
        if(zs->mode == mode)
        {
            svc->socketops = zs;
            break;
        }
    }

    if(svc->socketops == NULL)
    {
        zloge(ZTEXT("没找到对应的io模型, mode = %d"), mode);
        return NULL;
    }

    // 初始化
    svc->priv = svc->socketops->initialize(svc);
    if(svc->priv == NULL)
    {
        return NULL;
    }

    return svc;
}

void delete_zeus_svc(zeus_svc *svc)
{
    free(svc);
}

void zeus_svc_register_new_data_callback(zeus_svc *svc, zeus_socket_new_data_func callback, void *userdata)
{
    svc->on_client_new_data = callback;
    svc->userdata = userdata;
}

void zeus_svc_start(zeus_svc *svc)
{
    // 启动IO模型
    int rc = svc->socketops->start(svc);
}

void zeus_svc_stop(zeus_svc *svc)
{
    svc->socketops->stop(svc);
}

