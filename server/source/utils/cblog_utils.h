/***********************************************************************************
 * @ file    : steak_utils.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2023.05.16 15:56
 * @ brief   : ¹¤¾ßº¯Êý
 ************************************************************************************/

#ifndef __STEAK_UTILS_H__
#define __STEAK_UTILS_H__

#include <stdint.h>

#include <libY.h>

#define new_object(struct_type, var) struct_type *var = (struct_type *)calloc(1, sizeof(struct_type));		\
									if(var == NULL)															\
									{																		\
										YLOGE("calloc failed, %s", strerror(errno));						\
										return STEAK_ERR_NO_MEM;											\
									}																		\

#define new_array(struct_type, var, count) struct_type **var = (struct_type **)calloc(count, sizeof(struct_type));			\
											if(var == NULL)																	\
											{																				\
												YLOGE("calloc failed, %s", strerror(errno));								\
												return STEAK_ERR_NO_MEM;													\
											}																				\


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif