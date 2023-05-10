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

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#elif (defined(ENV_UNIX))
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include <libY.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
typedef SOCKET steak_socket;
#elif (defined(ENV_UNIX))
typedef int steak_socket;
#endif

#ifdef __cplusplus
extern "C" {
#endif

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

#ifdef __cplusplus
}
#endif

#endif
