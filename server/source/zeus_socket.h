/***********************************************************************************
 * @ file    : zeus_poll.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.26 21:00
 * @ brief   : 封装socket网络模型（select，epoll，IOCP..）
 ************************************************************************************/

#ifndef __ZEUS_SOCKET_H__
#define __ZEUS_SOCKET_H__

#ifdef ZEUS_WIN32
#include <Windows.h>
#include <WinSock2.h>
#endif

#include "zeus.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef void(*zeus_socket_new_data_func)(zeus *z, SOCKET fd, const char *data, size_t datasize, void *userdata);

    typedef enum 
    {
        ZSOCK_MODE_SELECT,
        ZSOCK_MODE_EPOLL,
        ZSOCK_MODE_IOCP
    }ZEUS_SOCKET_MODE;

    typedef struct zeus_socket_ops_s 
    {
        ZEUS_SOCKET_MODE mode;
        void*(*initialize)(zeus_socket *poll);
        SOCKET(*create_socket)(zeus_socket *poll);
        void(*add_client)(zeus_socket *poll, SOCKET skfd);
        void(*remove_client)(zeus_socket *poll, SOCKET skfd);
    }zeus_socket_ops;

    typedef struct zeus_socket_s
    {
        SOCKET *skfd;
        zeus_socket_ops *operations;
        zeus_socket_new_data_func cb_new_data;
        void *private;
        void *userdata;
    }zeus_socket;

    int new_zeus_socket(zeus_socket **zsocket, ZEUS_SOCKET_MODE mode);
    int delete_zeus_socket(zeus_socket *zsocket);
    void zeus_socket_new_data_callback(zeus_socket *zsocket, zeus_svc_new_data_func callback, void *userdata);

#ifdef __cplusplus
}
#endif

#endif

