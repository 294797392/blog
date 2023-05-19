/***********************************************************************************
 * @ file    : cblog_string.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.11 16:45
 * @ brief   : ×Ö·û´®¹¤¾ßº¯Êý
 ************************************************************************************/

#ifndef __CBLOG_STRING_H__
#define __CBLOG_STRING_H__

#include <stdint.h>

#include <libY.h>

#include "cblog_types.h"

#ifdef __cplusplus
extern "C" {
#endif

	int cblog_string_casecmp(const char *str1, const char *str2, int ncount);

	void cblog_string_print(char *str, cblog_string *cbstr);
	void cblog_string_print2(char *prepend, char *str, cblog_string *cbstr);
	void cblog_string_print_header(char *str, cblog_http_header *header);

#ifdef __cplusplus
}
#endif

#endif