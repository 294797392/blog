/***********************************************************************************
 * @ file    : steak_type.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.10 19:06
 * @ brief   : steak自定义类型
 ************************************************************************************/

#ifndef __STEAK_TYPE_H__
#define __STEAK_TYPE_H__

#ifdef __cplusplus
extern "C" {
#endif

	typedef struct steak_string_s steak_string;
	struct steak_string_s
	{
		char *str;
		int len;
	};

#ifdef __cplusplus
}
#endif

#endif