/***********************************************************************************
 * @ file    : cblog_buffer.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.22 13:57
 * @ brief   : 缓冲区管理
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
		/// buffer地址
		/// 该地址有可能会通过realloc函数进行扩容
		/// </summary>
		char *pdata;

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
	cblog_buffer *new_cblog_buffer(int size);

	/*
	 * 描述：
	 * 释放缓冲区对象
	 *
	 * 参数：
	 * @buffer：要释放的缓冲区对象
	 */
	void free_cblog_buffer(cblog_buffer *buffer);

	/*
	 * 描述：
	 * 重置缓冲区对象
	 * 缓冲区指针移动到缓冲区开头
	 *
	 * 参数：
	 * @buffer：要重置的缓冲区对象
	 */
	void cblog_buffer_reset(cblog_buffer *buffer);

	/*
	 * 描述：
	 * 接收socket数据并保存到缓冲区
	 * 确保缓冲区剩余可用空间大于等于size长度
	 * 如果小于size，那么会自动扩容
	 *
	 * 参数：
	 * @buffer：要操作的缓冲区对象
	 * @sock：socket句柄
	 *
	 * 返回值：
	 * 同recv返回值
	 */
	int cblog_buffer_recv_socket(cblog_buffer *buffer, cblog_socket sock);

	/*
	 * 描述：
	 * 向socket发送数据
	 *
	 * 参数：
	 * @buffer：要操作的缓冲区对象
	 * @sock：socket句柄
	 *
	 * 返回值：
	 * 同send返回值
	 */
	int cblog_buffer_send_socket(cblog_buffer *buffer, cblog_socket sock);

	/// <summary>
	/// 往缓冲区里写数据
	/// 如果缓冲区不够，那么会自动扩容缓冲区
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