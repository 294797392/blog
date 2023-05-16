/***********************************************************************************
 * @ file    : factory.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.13 08:30
 * @ brief   : 用来创建对象和释放对象的工厂
 ************************************************************************************/

#ifndef __STEAK_FACTORY_H__
#define __STEAK_FACTORY_H__

#include "session.h"
#include "connection.h"

#ifdef __cplusplus
extern "C" {
#endif

	steak_session *new_session(steak_socket sock);
	void free_session(steak_session *session);

#ifdef __cplusplus
}
#endif

#endif