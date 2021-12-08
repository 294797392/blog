#include <libY/Yfirstinclude.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef Y_API_WIN32
#include <WinSock2.h>
#include <Windows.h>
#elif Y_API_UNIX
#include <unistd.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#endif

// libY
#include <libY/Ylog.h>
#include <libY/Ythread.h>
#include <libY/Ynet.h>
#include <libY/Ylist.h>
#include <libY/Yqueue.h>
#include <libY/Ypool.h>

// mm
#include "mm_errno.h"
#include "mm_config.h"

#ifdef Y_ENV_WIN32
#pragma comment(lib, "Ws2_32.lib")
#endif

#define CATE YTEXT("mm")

typedef struct mm_thread_s
{
    // 当前线程是否空闲
    int idle;

    Ythread *thread;
    Ysocket client_socket;

#ifdef Y_API_WIN32
    CRITICAL_SECTION lock;	    // 临界区对象
    HANDLE event;               // 处理客户端请求的事件对象
#elif Y_API_UNIX
    pthread_mutex_t queue_mutex;
#endif
}mm_thread;

static Ythread *svc_thread;
static Ysocket ysock;
static int thread_num;
static Ythread **handler_threads;

static mm_thread *lookup_idle_thread()
{
    for (int i = 0; i < thread_num; i++)
    {
        mm_thread *thread = handler_threads[i];
        if (thread->idle == 1)
        {
            return thread;
        }
    }

    return NULL;
}

static void svc_thread_entry(void *userdata)
{
    while(1)
    {
        struct sockaddr_in client_addr;
        Ysocket client_socket = 0;
#ifdef Y_API_WIN32
        int sin_size = sizeof(struct sockaddr_in);
#elif Y_API_UNIX
        unsigned int sin_size = sizeof(struct sockaddr_in);
#endif
        if ((client_socket = accept(ysock, (struct sockaddr *)&client_addr, &sin_size)) < 0)
        {
            YLOGCE(CATE, YTEXT("accept client failed, %d"), Y_socket_error());
            continue;
        }

        mm_thread *thread = lookup_idle_thread();
        if (thread == NULL)
        {
            // 服务器处理不过来了..
        }
        else
        {
            thread->client_socket = client_socket;
            thread->idle = 0;
            SetEvent(thread->event);
        }
    }
}

static void handler_thread_entry(void *userdata)
{
    mm_thread *thread = (mm_thread *)userdata;

    while (1)
    {
        WaitForSingleObject(thread->event, INFINITE);

        Ysocket csk = thread->client_socket;

        YLOGCI(CATE, YTEXT("handle client request..."));

        thread->idle = 1;

        Y_close_socket(csk);
    }
}

static mm_thread **init_handler_threads(int thread_num)
{
    mm_thread **threads = (mm_thread **)calloc(thread_num, sizeof(mm_thread *));

    for(int i = 0; i < thread_num; i++)
    {
        mm_thread *thread = (mm_thread *)calloc(1, sizeof(mm_thread));
#ifdef Y_API_WIN32
        InitializeCriticalSection(&thread->lock);
        thread->event = CreateEvent(NULL, FALSE, FALSE, NULL);
#elif Y_API_UNIX
        pthread_mutex_init(&queue->lock, NULL);
#endif
        thread->idle = 1;
        thread->thread = Y_create_thread(handler_thread_entry, thread);
        threads[i] = thread;
    }

    return threads;
}

int main(int argc, char **argv)
{
    Y_log_global_init();
    Y_initnet();
    mmcfg_init();

    // 实例化变量
    thread_num = mmcfg_get_thread_num();
    handler_threads = init_handler_threads(thread_num);

    // 创建服务器
    ysock = Y_create_tcp_svc(NULL, mmcfg_get_port());
    svc_thread = Y_create_thread(svc_thread_entry, NULL);

    YLOGI(YTEXT("mmsvc is running..."));

    while (1)
    {
        char line[1024] = {'\0'};
        fgets(line, sizeof(line), stdin);
    }

    return 0;
}