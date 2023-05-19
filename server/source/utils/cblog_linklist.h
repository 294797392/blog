/***********************************************************************************
 * @ file    : cblog_linklist.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.18 18:00
 * @ brief   : socket�������������
 ************************************************************************************/

#ifndef __CBLOG_BUFFER_H__
#define __CBLOG_BUFFER_H__

#include "steak_socket.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct cblog_sockbuf_s cblog_sockbuf;

	struct cblog_sockbuf_s
	{
		/// <summary>
		/// socket���
		/// </summary>
		cblog_socket sock;

		/// <summary>
		/// bufferָ��
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
	cblog_sockbuf *new_cblog_sockbuf(cblog_socket sock, int size);

	/*
	 * ������
	 * �ͷŻ���������
	 *
	 * ������
	 * @buffer��Ҫ�ͷŵĻ���������
	 */
	void free_cblog_sockbuf(cblog_sockbuf *buffer);

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
	int cblog_sockbuf_recv(cblog_sockbuf *buffer);

	/*
	 * ������
	 * ���û���������
	 * ������ָ���ƶ�����������ͷ
	 *
	 * ������
	 * @buffer��Ҫ���õĻ���������
	 */
	void cblog_sockbuf_reset(cblog_sockbuf *buffer);

#ifdef __cplusplus
}
#endif

#endif