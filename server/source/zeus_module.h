/***********************************************************************************
 * @ file    : zeus_module.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.08.08 22:57
 * @ brief   : 封装一个抽象的模块
 ************************************************************************************/

#ifndef __ZEUS_MODULE_H__
#define __ZEUS_MODULE_H__

#include "zeus_string.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct zeus_kvpair_s
    {
        zchar *key;
        zchar *value;
    }zeus_kvpair;

    typedef struct zeus_module_s zeus_module;

    // 核心模块定义
    struct zeus_module_s
    {
        void *context;
        zeus_kvpair *settings;
        int(*initialize)(zeus_module *module);
        void(*release)(zeus_module *module);
    };

#ifdef __cplusplus
}
#endif

#endif