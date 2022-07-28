/***********************************************************************************
 * @ file    : zeus_svc.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.26 21:00
 * @ brief   : HTTP服务的实现
 *              1. 对客户端连接进行管理
 *              2. 接收客户端数据并回调给外部模块
 ************************************************************************************/

#ifndef __ZEUS_SVC_H__
#define __ZEUS_SVC_H__

#ifdef ZEUS_WIN32
#include <WinSock2.h>
#include <Windows.h>
#endif

#include "zeus_parser.h"
#include "zeus.h"

#ifdef __cplusplus
extern "C" {
#endif

	zeus_svc *new_zeus_svc(int port, ZEUS_SOCKET_MODE mode);
	void delete_zeus_svc(zeus_svc *zs);
	void zeus_svc_register_new_data_callback(zeus_svc *zs, zeus_socket_new_data_func callback, void *userdata);
	void zeus_svc_start(zeus_svc *zs);
	void zeus_svc_stop(zeus_svc *zs);

#ifdef __cplusplus
}
#endif

#endif

