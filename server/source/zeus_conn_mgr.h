/***********************************************************************************
 * @ file    : zeus_conn_mgr.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.23 23:30
 * @ brief   : 客户端连接管理器
 ************************************************************************************/

#ifndef __ZEUS_CONN_MGR_H__
#define __ZEUS_CONN_MGR_H__

#include "zeus.h"

typedef struct zeus_conn_mgr_s
{

    zeus_conn **conns;

}zeus_conn_mgr;


zeus_conn_mgr *new_zeus_conn_mgr();
void zeus_conn_mgr_


#endif