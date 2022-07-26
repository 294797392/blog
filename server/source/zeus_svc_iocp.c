/***********************************************************************************
 * @ file    : zeus_svc_iocp.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.26 21:07
 * @ brief   : 使用Windows的IO完成端口技术实现HTTP服务器
 ************************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Windows.h>
#include <WinSock2.h>
#include <MSWSock.h>

#include "zeus_errno.h"
#include "zeus_log.h"
#include "zeus_os.h"
#include "zeus_svc.h"

struct zeus_svc_s
{
    HANDLE iocp;
    SOCKET skfd;
};

int new_zeus_svc(int port)
{
    // 1. 首先创建一个空的IOCP对象
    HANDLE iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if(iocp == NULL)
    {
        zprintf(ZTEXT("创建IOCP端口失败, %d"), GetLastError());
        return ZERR_FAIL;
    }

    // 2. 创建Socket对象，注意，创建的Socket必须有WSA_FLAG_OVERLAPPED才可以和一个IOCP对象进行关联
    // 如果要使用IOCP，必须使用WSASocket函数，并设置标识位WSA_FLAG_OVERLAPPED
    SOCKET skfd = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if(skfd < 0)
    {
        zprintf(ZTEXT("创建Socket失败, %d"), WSAGetLastError());
        return ZERR_FAIL;
    }

    // 设置Socket选项，当程序异常退出之后再次打开程序的时候端口还可以继续使用
    int on = 1;
    if(setsockopt(skfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {  
        zprintf(ZTEXT("setsockopt失败, SO_REUSEADDR, %d"), WSAGetLastError());
		return ZERR_FAIL;
    }

    // 3. 将socket与IOCP对象关联
    if(CreateIoCompletionPort((HANDLE)skfd, iocp, NULL, 0) == NULL)
    {
        zprintf(ZTEXT("socket关联IOCP对象失败, %d"), GetLastError());
        return ZERR_FAIL;
    }

    // 设置要侦听的网络接口和端口
    struct sockaddr_in bdaddr;
	bdaddr.sin_family = PF_INET;
	bdaddr.sin_port = htons(port);
	bdaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(bdaddr.sin_zero), 0, sizeof(bdaddr.sin_zero));

    // 绑定
    if(bind(skfd, (struct sockaddr*)&bdaddr, sizeof(struct sockaddr)) < 0)
    {
        zprintf(ZTEXT("bind失败, %d"), WSAGetLastError());
		return ZERR_FAIL;
    }

    // 开始监听
	if(listen(skfd, 5) < 0)
	{
        zprintf(ZTEXT("listen失败, %d"), WSAGetLastError());
		return ZERR_FAIL;
	}

    zeus_svc *svc = (zeus_svc*)zeus_calloc(1, sizeof(zeus_svc));
    svc->iocp = iocp;
    svc->skfd = skfd;
    return svc;
}

int delete_zeus_svc()
{}

int zeus_svc_start()
{}
