/***********************************************************************************
 * @ file    : zeus_os.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.23 8:21
 * @ brief   : 封装不同操作系统下的API
 ************************************************************************************/

#ifndef __ZEUS_MISC_H__
#define __ZEUS_MISC_H__


#ifdef ZEUS_WIN32
#endif

int zeus_os_error();
void *zeus_calloc(int count, int size);
void zeus_free(void *mem);

#endif