/***********************************************************************************
 * @ file    : cblog_socket.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.10 10:46
 * @ brief   : socket工具方法
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
		/// socket句柄
		/// </summary>
		cblog_socket sock;

		/// <summary>
		/// buffer地址
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
	cblog_socket_buffer *new_cblog_sockbuf(cblog_socket sock, int size);

	/*
	 * 描述：
	 * 释放缓冲区对象
	 *
	 * 参数：
	 * @buffer：要释放的缓冲区对象
	 */
	void free_cblog_sockbuf(cblog_socket_buffer *buffer);

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
	int cblog_sockbuf_recv(cblog_socket_buffer *buffer);

	/*
	 * 描述：
	 * 重置缓冲区对象
	 * 缓冲区指针移动到缓冲区开头
	 *
	 * 参数：
	 * @buffer：要重置的缓冲区对象
	 */
	void cblog_sockbuf_reset(cblog_socket_buffer *buffer);









	/*
	 * 描述：
	 * 初始化socket
	 */
	int cblog_socket_init();

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
	int cblog_socket_get_avaliable_size(cblog_socket sock);
	

	/*
	 * 描述：
	 * 设置一个套接字的读写模式为异步
	 * 读写该套接字不会阻塞
	 *
	 * 参数：
	 * @sock：要设置的socket对象
	 *
	 * 返回值：
	 * CBLOG_ERR
	 */
	int cblog_socket_set_nonblock(cblog_socket sock);

	void cblog_socket_close(cblog_socket sock);

	/*
	 * 描述：
	 * 获取socket错误码
	 */
	int cblog_socket_error();



#ifdef __cplusplus
}
#endif

#endif
