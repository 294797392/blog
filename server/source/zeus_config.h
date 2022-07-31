/***********************************************************************************
 * @ file    : zeus_config.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.23 8:21
 * @ brief   : 服务器配置文件的加载和保存
 ************************************************************************************/

#ifndef __ZEUS_CONFIG_H__
#define __ZEUS_CONFIG_H__

typedef struct zeus_config_s
{
	char listen_addr[128];
	int listen_port;

	// 工作者线程的数量
	int num_worker;

	int event_driver_type;
}zeus_config;

zeus_config * zeus_pase_config(wchar_t *path);

#endif
