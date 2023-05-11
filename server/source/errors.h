#ifndef __ERRORS_H__
#define __ERRORS_H__

#define STEAK_ERR_OK							0
#define STEAK_ERR_NO_MEM						1
#define STEAK_ERR_FAILED						2
#define STEAK_ERR_REQUEST_INVALID				3				// 不存在content-length
#define STEAK_ERR_REQUEST_COMPLETED				4				// HTTP请求解析完毕

#endif