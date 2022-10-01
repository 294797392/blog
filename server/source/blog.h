/***********************************************************************************
 * @ file    : blog.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.09.29 21:24
 * @ brief   : 定义一些公共数据模型
 ************************************************************************************/

#ifndef __BLOG_H__
#define __BLOG_H__

#include <libY.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct service_s service;
    typedef struct service_conf_s service_conf;
    typedef struct event_s event;
    typedef struct event_poll_s event_poll;
    typedef struct event_poll_actions_s event_poll_actions;
    typedef struct listening_s listening;

    typedef int (*process_event_func)(event_poll *poll, event* evt);
    typedef int (*init_event_poll_func)(cJSON *config, void **ctx);
    typedef int (*add_event_func)(event_poll *poll, event *evt);
    typedef int (*del_event_func)(event_poll *poll, event *evt);
    typedef int (*poll_events_func)(event_poll *poll);

    struct event_s
    {
        int fd;                                 // 被监控的文件描述符
        int read;                               // 该文件描述符是否可读
        int write;                              // 该文件描述符是否可写
        process_event_func process;             // 处理该事件的处理器
    };

    struct event_poll_s
    {
        Ylist *event_list;                      // 所有事件的列表
        void *pollctx;                          // event_poll_actions使用的上下文信息
        event_poll_actions *actions;            // 轮询操作
        Yqueue *event_queue;                    // 待处理的事件
    };

    struct event_poll_actions_s
    {
        const char *name;
        init_event_poll_func init;
        add_event_func add_event;
        del_event_func del_event;
        poll_events_func poll;
    };

    struct listening_s
    {
        int port;
        YCHAR *root;

        int fd;
    };

    struct service_s
    {
        event_poll *poller;
        event_poll_actions *poll_actions;
        Ylist *listenings;
    };

    struct service_conf_s
    {
        int port;
        YCHAR *root;
    };

#ifdef __cplusplus
}
#endif

#endif

