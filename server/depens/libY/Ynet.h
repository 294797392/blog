/***********************************************************************************
 * @ file    : Ynet.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2021.12.07 16:26
 * @ brief   : 提供一些通用的关于socket和网络的函数
 ************************************************************************************/

#ifndef __YNET_H__
#define __YNET_H__

#include "Yfirstinclude.h"

#include "Ybase.h"

#ifdef Y_API_WIN32
typedef SOCKET Ysocket;
#elif Y_API_UNIX
typedef int Ysocket;
#endif

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * 描述：
	 * 初始化网络环境，该函数在整个应用程序里调用一次即可
	 */
	YAPI void Y_initnet();

	/*
	 * 描述：
	 * 断开并释放socket连接
	 *
	 * 参数：
	 * @socket：socket句柄
	 */
	YAPI void Y_close_socket(Ysocket s);

	/*
	 * 描述：
	 * 从一个文件描述符里读取数据，直到读到len大小或者失败才返回
	 *
	 * 参数：
	 * @s：要读取的文件描述符
	 * @data：保存读取到的数据的缓冲区
	 * @len：缓冲区长度
	 *
	 * 返回值：
	 * 读取成功返回0，读取失败返回-1
	 */
	YAPI int Y_read_socket(Ysocket s, char *data, size_t len);

	/*
	 * 描述：
	 * 向一个文件描述符里写入数据，直到写完len大小或者失败才返回
	 *
	 * 参数：
	 * @fd：要写入的文件描述符
	 * @data：要写入的数据地址
	 * @len：要写入的数据大小
	 *
	 * 返回值：
	 * 写入成功返回0，写入失败返回-1
	 */
	YAPI int Y_write_socket(Ysocket s, char *data, size_t len);

	/*
	 * 描述：
	 * 获取不同平台下的socket错误编码
	 */
	YAPI int Y_socket_error();

	/*
	 * 描述：
	 * 创建一个TCP类型的socket并开始监听某个端口
	 *
	 * 参数：
	 * @bindto：要绑定到的网卡的IP地址，默认值可以填NULL，NULL的意思就是监听所有的网卡地址
	 * @port：要监听的端口号
	 *
	 * 返回值：
	 * socket实例
	 */
	YAPI Ysocket Y_create_tcp_svc(const char *bindto, int port);

#ifdef __cplusplus
}
#endif


#endif