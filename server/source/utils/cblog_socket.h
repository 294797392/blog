/***********************************************************************************
 * @ file    : cblog_socket.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.10 10:46
 * @ brief   : socket���߷���
 ************************************************************************************/

#ifndef __CBLOG_SOCKET_H__
#define __CBLOG_SOCKET_H__

#include <stdint.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#elif (defined(ENV_UNIX))
#endif

#include <libY.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
typedef SOCKET cblog_socket;
#elif (defined(ENV_UNIX))
typedef int steak_socket;
#endif

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct cblog_socket_buffer_s cblog_socket_buffer;
	struct cblog_socket_buffer_s
	{
		/// <summary>
		/// socket���
		/// </summary>
		cblog_socket sock;

		/// <summary>
		/// buffer��ַ
		/// </summary>
		char *ptr;

		/// <summary>
		/// buffer�ܳ���
		/// </summary>
		int size;

		/// <summary>
		/// ������ʣ�೤��
		/// </summary>
		int left;

		/// <summary>
		/// дƫ����
		/// </summary>
		int offset;
	};

	/*
	 * ������
	 * �½�һ������������
	 *
	 * ������
	 * @sock��Ҫ�����socket����
	 * @size������������
	 *
	 * ����ֵ��
	 * ����������
	 */
	cblog_socket_buffer *new_cblog_sockbuf(cblog_socket sock, int size);

	/*
	 * ������
	 * �ͷŻ���������
	 *
	 * ������
	 * @buffer��Ҫ�ͷŵĻ���������
	 */
	void free_cblog_sockbuf(cblog_socket_buffer *buffer);

	/*
	 * ������
	 * ȷ��������ʣ����ÿռ���ڵ���size����
	 * ���С��size����ô���Զ�����
	 *
	 * ������
	 * @buffer��Ҫ�����Ļ���������
	 * @size������������
	 *
	 * ����ֵ��
	 * ͬrecv����ֵ
	 */
	int cblog_sockbuf_recv(cblog_socket_buffer *buffer);

	/*
	 * ������
	 * ���û���������
	 * ������ָ���ƶ�����������ͷ
	 *
	 * ������
	 * @buffer��Ҫ���õĻ���������
	 */
	void cblog_sockbuf_reset(cblog_socket_buffer *buffer);









	/*
	 * ������
	 * ��ʼ��socket
	 */
	int cblog_socket_init();

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
	int cblog_socket_get_avaliable_size(cblog_socket sock);
	

	/*
	 * ������
	 * ����һ���׽��ֵĶ�дģʽΪ�첽
	 * ��д���׽��ֲ�������
	 *
	 * ������
	 * @sock��Ҫ���õ�socket����
	 *
	 * ����ֵ��
	 * CBLOG_ERR
	 */
	int cblog_socket_set_nonblock(cblog_socket sock);

	void cblog_socket_close(cblog_socket sock);

	/*
	 * ������
	 * ��ȡsocket������
	 */
	int cblog_socket_error();



#ifdef __cplusplus
}
#endif

#endif
