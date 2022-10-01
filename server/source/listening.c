#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <libY.h>
#include <cJSON.h>
#include <cJSON_Helper.h>

#include "listening.h"
#include "default.h"
#include "blog.h"

Ylist *create_listens(cJSON *config)
{
    Ylist *listens = (Ylist*)Y_create_list();

    cJSON *listens_cfg = cJSON_GetObjectItem(config, "listens");
    if(listens_cfg == NULL)
    {
        // 配置文件里没有配置listens节点
        return listens;
    }

    int num_listens = cJSON_GetArraySize(listens_cfg);
    for(int i = 0; i < num_listens; i++)
    {
        cJSON *listen_cfg = cJSON_GetArrayItem(listens_cfg, i);
        YCHAR *root = (YCHAR*)Ycalloc(DEF_ROOT_LEN, sizeof(YCHAR));
        int port = cJSON_GetIntValue2(listen_cfg, "port", DEF_PORT + i);
        cJSON_GetStringValue2(listen_cfg, "root", root, DEF_ROOT_LEN, "public");

        listening *listen = (listening*)Ycalloc(1, sizeof(listening));
        listen->port = port;
        listen->root = root;
        Y_list_add(listens, listen);
    }

    return listens;
}

int open_listens(Ylist *listens)
{

}


