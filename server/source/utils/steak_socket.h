/***********************************************************************************
 * @ file    : steak_socket.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.10 10:46
 * @ brief   : socket���߷���
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
	 * ������
	 * ��ʼ��socket
	 */
	int steak_socket_init();

	/*
	 * ������
	 * ��ȡ�Ѿ���������տɹ���ȡ��������
	 *
	 * ������
	 * @sock��Ҫ��ȡ��socket����
	 *
	 * ����ֵ��
	 * �ɶ�ȡ��������
	 */
	int steak_socket_get_avaliable_size(steak_socket sock);

	void steak_socket_close(steak_socket sock);

	/*
	 * ������
	 * ��ȡsocket������
	 */
	int steak_socket_error();

#ifdef __cplusplus
}
#endif

#endif
