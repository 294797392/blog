/***********************************************************************************
 * @ file    : cblog_default.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.12.01 21:39
 * @ brief   : 定义应用程序里的默认值
 ************************************************************************************/

#ifndef __CBLOG_DEFAULT__H__
#define __CBLOG_DEFAULT__H__


#define CBLOG_WELCOME_MESSAGE								"welcome to cblog!"


// 最多的网络连接的数量
#define STEAK_DEFAULT_MAX_SESSIONS							1024

// 默认标头缓冲区大小。64个乘256长度
#define CBLOG_DEFAULT_HEADER_BUFFER_SIZE					16384

// HTTP请求报文的缓冲区大小。默认64k
#define CBLOG_DEFAULT_REQUEST_BUFFER_SIZE					65536

// HTTP响应报文的缓冲区大小。默认64k
#define CBLOG_DEFAULT_RESPONSE_BUFFER_SIZE					65535

// HTTP响应BODY的缓冲区大小。默认64k
#define CBLOG_DEFAULT_RESPONSE_BODY_BUFFER_SIZE				65535

// 轮询事件的超时时间，单位是毫秒
#define STEAK_DEFAULT_POLL_TIMEOUT							50

// 默认的HTTP根目录
#define STEAK_DEFAULT_ROOT									"/"

// 默认监听端口
#define STEAK_DEFAULT_LISTEN_PORT							1018

#define STEAK_DEFAULT_CONFIG_FILE							"service.json"

#endif