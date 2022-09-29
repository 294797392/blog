/***********************************************************************************
 * @ file    : Yfile.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.09.09 06:43
 * @ brief   : 封装文件操作，兼容宽字符和单字节字符
 * @ remark  ：
 ************************************************************************************/

#ifndef __YFILE_H__
#define __YFILE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "Y.h"

#ifdef __cplusplus
extern "C" {
#endif

	// 存储文件信息
	typedef struct Yfstat_s
	{
		int exist;
		uint64_t length;
	}Yfstat;

	/*
	 * 描述：
	 * 以字节为单位返回文件大小
	 *
	 * 参数：
	 * @file_path：要读取的文件的完整路径
	 *
	 * 返回值：
	 * 文件大小，以字节为单位
	 */
	YAPI int Y_file_stat(const YCHAR *file_path, Yfstat *stat);


	/*
	 * 描述：
	 * 读取一个文件里的所有字节
	 *
	 * 参数：
	 * @file_path：要读取的文件的完整路径
	 * @bytes：字节缓冲区
	 *
	 * 返回值：
	 * 文件内容的长度
	 */
	YAPI int Y_file_readbytes(const YCHAR *file_path, YBYTE **bytes, uint64_t *size);

	/*
	 * 描述：
	 * 释放使用Y_file_readall函数开辟的内存空间
	 *
	 * 参数：
	 * @content：要释放的内存空间
	 */
	YAPI void Y_file_free(char *content);

#ifdef __cplusplus
}
#endif

#endif