/***********************************************************************************
 * @ file    : cblog_errors.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : ???
 * @ brief   : 定义cblog的错误码
 ************************************************************************************/

#ifndef __CBLOG_ERRORS_H__
#define __CBLOG_ERRORS_H__

#define CBLOG_ERR_OK							0
#define CBLOG_ERR_NO_MEM						1
#define CBLOG_ERR_FAILED						2
#define CBLOG_ERR_INVALID_REQUEST				3				// 不存在content-length
#define CBLOG_ERR_SYSERR						4				// 系统错误
#define CBLOG_ERR_INVALID_CONFIG				5				// 配置文件格式不正确

#endif