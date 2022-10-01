/***********************************************************************************
 * @ file    : default.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.09.30 07:28
 * @ brief   : 定义默认值
 ************************************************************************************/

#ifndef __DEFAULT_H__
#define __DEFAULT_H__

#ifdef __cplusplus
extern "C" {
#endif

// 默认端口号
// 如果有多个监听，那么在默认端口号的基础上加1
#define DEF_PORT                                        1018

// 默认的HTTP根目录
#define DEF_ROOT_LEN                                    256

#define DEF_CONFIG_PATH                                 "config.json"

#define DEF_POLL_ACTION                                 "select"


#ifdef __cplusplus
}
#endif

#endif

