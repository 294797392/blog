/***********************************************************************************
 * @ file    : cblog_linklist.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.18 18:00
 * @ brief   : socket缓冲区管理对象
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
		/// socket句柄
		/// </summary>
		cblog_socket sock;

		/// <summary>
		/// buffer指针
		/// </summary>
		char *ptr;

		/// <summary>
		/// buffer总长度
		/// </summary>
		int size;

		/// <summary>
		/// 缓冲区剩余长度
		/// </summary>
		int left;

		/// <summary>
		/// 写偏移量
		/// </summary>
		int offset;
	};

	/*
	 * 描述：
	 * 新建一个缓冲区对象
	 *
	 * 参数：
	 * @sock：要管理的socket对象
	 * @size：缓冲区长度
	 *
	 * 返回值：
	 * 缓冲区对象
	 */
	cblog_sockbuf *new_cblog_sockbuf(cblog_socket sock, int size);

	/*
	 * 描述：
	 * 释放缓冲区对象
	 *
	 * 参数：
	 * @buffer：要释放的缓冲区对象
	 */
	void free_cblog_sockbuf(cblog_sockbuf *buffer);

	/*
	 * 描述：
	 * 确保缓冲区剩余可用空间大于等于size长度
	 * 如果小于size，那么会自动扩容
	 *
	 * 参数：
	 * @buffer：要操作的缓冲区对象
	 * @size：缓冲区长度
	 *
	 * 返回值：
	 * 同recv返回值
	 */
	int cblog_sockbuf_recv(cblog_sockbuf *buffer);

	/*
	 * 描述：
	 * 重置缓冲区对象
	 * 缓冲区指针移动到缓冲区开头
	 *
	 * 参数：
	 * @buffer：要重置的缓冲区对象
	 */
	void cblog_sockbuf_reset(cblog_sockbuf *buffer);

#ifdef __cplusplus
}
#endif

#endif