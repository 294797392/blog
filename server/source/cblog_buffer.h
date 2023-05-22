/***********************************************************************************
 * @ file    : cblog_buffer.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.22 13:57
 * @ brief   : ����������
 ************************************************************************************/

#ifndef __CBLOG_BUFFER_H__
#define __CBLOG_BUFFER_H__

#include <stdint.h>

#include <libY.h>

#include "cblog_socket.h"

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct cblog_buffer_s cblog_buffer;
	struct cblog_buffer_s
	{
		/// <summary>
		/// buffer��ַ
		/// �õ�ַ�п��ܻ�ͨ��realloc������������
		/// </summary>
		char *pdata;

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
	cblog_buffer *new_cblog_buffer(int size);

	/*
	 * ������
	 * �ͷŻ���������
	 *
	 * ������
	 * @buffer��Ҫ�ͷŵĻ���������
	 */
	void free_cblog_buffer(cblog_buffer *buffer);

	/*
	 * ������
	 * ���û���������
	 * ������ָ���ƶ�����������ͷ
	 *
	 * ������
	 * @buffer��Ҫ���õĻ���������
	 */
	void cblog_buffer_reset(cblog_buffer *buffer);

	/*
	 * ������
	 * ����socket���ݲ����浽������
	 * ȷ��������ʣ����ÿռ���ڵ���size����
	 * ���С��size����ô���Զ�����
	 *
	 * ������
	 * @buffer��Ҫ�����Ļ���������
	 * @sock��socket���
	 *
	 * ����ֵ��
	 * ͬrecv����ֵ
	 */
	int cblog_buffer_recv_socket(cblog_buffer *buffer, cblog_socket sock);

	/*
	 * ������
	 * ��socket��������
	 *
	 * ������
	 * @buffer��Ҫ�����Ļ���������
	 * @sock��socket���
	 *
	 * ����ֵ��
	 * ͬsend����ֵ
	 */
	int cblog_buffer_send_socket(cblog_buffer *buffer, cblog_socket sock);

	/// <summary>
	/// ����������д����
	/// �����������������ô���Զ����ݻ�����
	/// </summary>
	/// <param name="buffer"></param>
	/// <param name="str"></param>
	/// <param name="len"></param>
	/// <returns></returns>
	int cblog_buffer_write(cblog_buffer *buffer, const char *str, int len);

#ifdef __cplusplus
}
#endif

#endif