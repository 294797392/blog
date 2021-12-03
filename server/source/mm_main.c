#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <WinSock2.h>

#include "mm_errno.h"
#include "mm_platform.h"
#include "mm_config.h"
#include "mm_main.h"

typedef struct mmsvc_s
{
    int skfd;
}mmsvc;

static MMSOCKET initsocket(int port, const char *bindto)
{
#ifdef MMP_WIN32
    WORD ver = MAKEWORD(1, 1);
    WSADATA wsadata;
    WSAStartup(ver, &wsadata);
#endif

    // 创建socket实例
	MMSOCKET fd = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in bdaddr;
	bdaddr.sin_family = PF_INET;
	bdaddr.sin_port = htons(port);
	bdaddr.sin_addr.s_addr = bindto == NULL ? INADDR_ANY : inet_addr(bindto);
	memset(&(bdaddr.sin_zero), 0, sizeof(bdaddr.sin_zero));

	// 绑定到指定网络接口
	if (bind(fd, (struct sockaddr *)&bdaddr, sizeof(struct sockaddr)) == -1)
	{
		return -1;
	}

    if(listen(fd, 5) < 0)
    {
        return -1;
    }

    return fd;
}

int main(int argc, char **argv)
{
    printf("hello mmsvc\n");

    mmcfg_init();

    mmsvc *svc = (mmsvc*)calloc(1, sizeof(mmsvc));
    svc->skfd = initsocket(mmcfg_get_port(), "0.0.0.0");

    while (1)
    {
        char line[1024] = {'\0'};
        fgets(line, sizeof(line), stdin);
    }
    
    return 0;
}
