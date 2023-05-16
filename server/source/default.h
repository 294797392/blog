/***********************************************************************************
 * @ file    : default.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.12.01 21:39
 * @ brief   : 定义应用程序里的默认值
 ************************************************************************************/

#ifndef __STEAK_DEFAULT__H__
#define __STEAK_DEFAULT__H__

// 最多的网络连接的数量
#define STEAK_DEFAULT_MAX_SESSIONS							1024

// 接收缓冲区的大小
#define STEAK_DEFAULT_HTTP_MSG_SIZE							16384

// 轮询事件的超时时间，单位是毫秒
#define STEAK_DEFAULT_POLL_TIMEOUT							50

// 默认的HTTP根目录
#define STEAK_DEFAULT_ROOT									"/"

// 默认监听端口
#define STEAK_DEFAULT_LISTEN_PORT							1018

#define STEAK_DEFAULT_CONFIG_FILE							"service.json"

#endif