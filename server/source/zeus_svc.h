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

#include "zeus_config.h"
#include "zeus_parser.h"
#include "zeus.h"

#ifdef __cplusplus
extern "C" {
#endif

	zeus_svc *new_zeus_svc(zeus_config *config);
	void free_zeus_svc(zeus_svc *svc);
	void zeus_svc_start(zeus_svc *svc);
	void zeus_svc_stop(zeus_svc *svc);

	//void zeus_svc_register_new_data_callback(zeus_svc *zs, zeus_socket_new_data_func callback, void *userdata);

#ifdef __cplusplus
}
#endif

#endif

