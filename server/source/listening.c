#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if (defined(Y_WIN32)) || (defined(Y_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#endif

#include <libY.h>
#include <cJSON.h>
#include <cJSON_Helper.h>

#include "platform.h"
#include "errors.h"
#include "listening.h"
#include "default.h"
#include "blog.h"

static void open_listen(Ylist *yl, void *item, void *userdata)
{
    listening *lsten = (listening*)item;
	int error = 0;
#if (defined(Y_WIN32)) || (defined(Y_MINGW))
	SOCKET s = -1;
#elif (defined(Y_UNIX))
    int s = -1;
#endif

	// 新建一个socket
	if((s = socket(PF_INET, SOCK_STREAM, 0)) == -1)
	{
		error = syserror();
        YLOGE(YTEXT("create tcp socket failed, %d"), error);
        return;
	}

	// 设置Socket选项，当程序异常退出之后再次打开程序的时候端口还可以继续使用
	int on = 1;
	if(setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		error = syserror();
		YLOGE(YTEXT("setsockopt failed SO_REUSEADDR, %d"), error);
		return NULL;
	}

	// 设置要侦听的网络接口和端口
	struct sockaddr_in bdaddr;
	bdaddr.sin_family = PF_INET;
	bdaddr.sin_port = htons(lsten->port);
	bdaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(bdaddr.sin_zero), 0, sizeof(bdaddr.sin_zero));

	// 绑定
	if(bind(s, (struct sockaddr *)&bdaddr, sizeof(struct sockaddr)) < 0)
	{
		error = syserror();
		YLOGE(YTEXT("bind failed, %d"), error);
		return NULL;
	}

    YLOGI(YTEXT("open listen success, port = %d, root = %s"), lsten->port, lsten->root);

    lsten->fd = s;
}

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
    Y_list_foreach(listens, open_listen, listens);

    return YERR_SUCCESS;
}


