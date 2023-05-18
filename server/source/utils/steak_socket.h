/***********************************************************************************
 * @ file    : steak_socket.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.10 10:46
 * @ brief   : socket工具方法
 ************************************************************************************/

#ifndef __STEAK_SOCKET_H__
#define __STEAK_SOCKET_H__

#include <stdint.h>

#include <libY.h>

#include "cblog_types.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * 描述：
	 * 初始化socket
	 */
	int steak_socket_init();

	/*
	 * 描述：
	 * 获取已经从网络接收可供读取的数据量
	 *
	 * 参数：
	 * @sock：要获取的socket对象
	 *
	 * 返回值：
	 * 可读取的数据量
	 */
	int steak_socket_get_avaliable_size(steak_socket sock);

	void steak_socket_close(steak_socket sock);

	/*
	 * 描述：
	 * 获取socket错误码
	 */
	int steak_socket_error();

#ifdef __cplusplus
}
#endif

#endif
