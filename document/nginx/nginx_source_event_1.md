## 缘由

最近想写一个属于自己的HTTP服务器，并把自己写的网站挂上去，但是没有什么开发HTTP的经验，很多问题不知道该如何处理，比如HTTP的客户端连接管理，HTTP长连接如何实现等等..，
此时就想到了nginx-一个开源的web服务器，先学习下它的源码，看它是怎么做的，然后可以模仿它的思路去写自己的服务器，这样就可以少走很多弯路。

nginx源码在逻辑上分为几个不同的模块，分别是event...

## 模块介绍

* ngx_init_cycle 
nginx源码中有一个核心结构体，叫做ngx_cycle_t，这个结构体里包含了nginx运行的时候所需要的所有的数据和对象

## nginx启动过程

首先从main函数开始，在core/nginx.c文件里，ngx_init_cycle，顾名思义，作用就是初始化ngx_cycle_t结构体，结构体里包含了所有的module，首先会调用ngx_cycle_modules对ngx_cycle_t结构体里的modules字段进行初始化，
modules字段里包含了nginx里所有的模块，然后创建核心模块（NGX_CORE_MODULE）的配置文件（module->create_conf），然后会调用module->init_conf，进行初始化配置文件，接着socket监听端口，然后调用ngx_init_modules，这个函数比较关键，会对nginx里的所有模块进行初始化，最后会对父子进程的一些东西进行处理。

这里主要关注ngx_cycle_modules和ngx_init_modules这两个函数，这两个函数都在ngx_module.c文件里

ngx_cycle_moudles:
```c
ngx_int_t
ngx_cycle_modules(ngx_cycle_t *cycle)
{
    /*
     * create a list of modules to be used for this cycle,
     * copy static modules to it
     */

    cycle->modules = ngx_pcalloc(cycle->pool, (ngx_max_module + 1)
                                              * sizeof(ngx_module_t *));
    if (cycle->modules == NULL) {
        return NGX_ERROR;
    }

    ngx_memcpy(cycle->modules, ngx_modules,
               ngx_modules_n * sizeof(ngx_module_t *));

    cycle->modules_n = ngx_modules_n;

    return NGX_OK;
}

```
代码很容易理解，就是把全局的ngx_modules拷贝到了cycle结构体的modules字段里。

ngx_init_modules：
```c
ngx_int_t
ngx_init_modules(ngx_cycle_t *cycle)
{
    ngx_uint_t  i;

    for (i = 0; cycle->modules[i]; i++) {
        if (cycle->modules[i]->init_module) {
            if (cycle->modules[i]->init_module(cycle) != NGX_OK) {
                return NGX_ERROR;
            }
        }
    }

    return NGX_OK;
}
这个函数里会调用所有模块的init_module函数指针，如果某个模块的init_module返回值不是NGX_OK，那么就会停止初始化```

模块的启动流程到这里就基本结束了，剩下的就是看每个模块是怎么实现的了。



## ngx_module_t结构体
nginx中所有的模块都使用ngx_module_t这个结构体来描述，下面是该结构体的定义：
```C
struct ngx_module_s {
    ngx_uint_t            ctx_index;
    ngx_uint_t            index;

    char                 *name;

    ngx_uint_t            spare0;
    ngx_uint_t            spare1;

    ngx_uint_t            version;
    const char           *signature;

    void                 *ctx;
    ngx_command_t        *commands;
    ngx_uint_t            type;

    ngx_int_t           (*init_master)(ngx_log_t *log);

    ngx_int_t           (*init_module)(ngx_cycle_t *cycle);

    ngx_int_t           (*init_process)(ngx_cycle_t *cycle);
    ngx_int_t           (*init_thread)(ngx_cycle_t *cycle);
    void                (*exit_thread)(ngx_cycle_t *cycle);
    void                (*exit_process)(ngx_cycle_t *cycle);

    void                (*exit_master)(ngx_cycle_t *cycle);

    uintptr_t             spare_hook0;
    uintptr_t             spare_hook1;
    uintptr_t             spare_hook2;
    uintptr_t             spare_hook3;
    uintptr_t             spare_hook4;
    uintptr_t             spare_hook5;
    uintptr_t             spare_hook6;
    uintptr_t             spare_hook7;
};
```
这个结构体里的字段还是稍微有点多的，不过也不复杂。
下面对这个结构体里的一些重要的字段进行说明：
* name：模块的名字
* ctx：模块的上下文信息
* commands：暂时没研究
* type：模块类型
NGX_CORE_MODULE：核心模块
NGX_CONF_MODULE：核心模块配置文件
NGX_EVENT_MODULE：事件模块
NGX_EVENT_CONF：事件模块配置文件
代码在core/ngx_conf_file.h文件里
* init_master：暂时没研究
* init_module：函数指针，在模块初始化的时候被调用（ngx_init_modules函数），作用是初始化该模块所需要使用的相关资源
* init_process：函数指针，在main函数里的ngx_single_process_cycle函数里会被调用

我们主要关注ngnix的启动流程。
其中与启动流程关联最大的字段就是init_module和init_process这两个函数指针，这两个函数指针会在ngx_init_cycle里被调用。
初始化的调用流程很简单，就是init_module -> init_process -> event_module.process_event

## event模块解析

event模块的主要作用是监控文件描述符的可读和可写的状态，ngx_event_t结构体用来封装一个文件描述符，event模块对ngx_event_t进行监控。
如果一个ngx_event_t可读或者可写，那么event模块就会调用ngx_event_t对应的handler去处理事件。
与event模块相关的代码在event目录里。ngx_event_xxx.c封装了不同ngx_event_t的handler实现，event/modules目录下对不同操作系统下的文件描述符的io模型进行了封装（比如select模型，windows下的iocp模型，linux下的epoll和poll模型），
所以nginx支持使用多种方式去对文件描述符进行监控和管理，扩展性很好。



在分析之前需要先分析event模块的两个核心结构体：
```C
typedef struct {
    ngx_int_t  (*add)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
    ngx_int_t  (*del)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);

    ngx_int_t  (*enable)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);
    ngx_int_t  (*disable)(ngx_event_t *ev, ngx_int_t event, ngx_uint_t flags);

    ngx_int_t  (*add_conn)(ngx_connection_t *c);
    ngx_int_t  (*del_conn)(ngx_connection_t *c, ngx_uint_t flags);

    ngx_int_t  (*notify)(ngx_event_handler_pt handler);

    ngx_int_t  (*process_events)(ngx_cycle_t *cycle, ngx_msec_t timer,
                                 ngx_uint_t flags);

    ngx_int_t  (*init)(ngx_cycle_t *cycle, ngx_msec_t timer);
    void       (*done)(ngx_cycle_t *cycle);
} ngx_event_actions_t;

typedef struct {
    ngx_str_t              *name;

    void                 *(*create_conf)(ngx_cycle_t *cycle);
    char                 *(*init_conf)(ngx_cycle_t *cycle, void *conf);

    ngx_event_actions_t     actions;
} ngx_event_module_t;
```
ngx_event_actions_t结构体管理ngx_event_t事件，下面介绍下比较重要的几个字段：
add：增加一个要监控的ngx_event_t
del：删除一个不需要监控的ngx_event_t
process_events：该函数在main函数中被周期性的调用，每调用一次，event模块就会监控一次模块里的所有的ngx_event_t，当某个ngx_event_t可读或者可写的时候，调用ngx_event_t对应的handler。




我们主要关注两个模块，分别是ngx_events_module和ngx_event_core_module，这两个模块的源码都在event/ngx_event.c文件里，
贴一下代码：
```C
ngx_module_t  ngx_events_module = {
    NGX_MODULE_V1,
    &ngx_events_module_ctx,                /* module context */
    ngx_events_commands,                   /* module directives */
    NGX_CORE_MODULE,                       /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};

ngx_module_t  ngx_event_core_module = {
    NGX_MODULE_V1,
    &ngx_event_core_module_ctx,            /* module context */
    ngx_event_core_commands,               /* module directives */
    NGX_EVENT_MODULE,                      /* module type */
    NULL,                                  /* init master */
    ngx_event_module_init,                 /* init module */
    ngx_event_process_init,                /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};
```
目前我们只关心ngx_event_core_module
其中ngx_event_core_module_ctx是模块的上下文信息，上下文信息就是该模块在运行的时候所需要的信息，
ngx_event_core_commands暂时没研究。
通过module type字段可以看出来它是一个事件模块（NGX_EVENT_MODULE），这里要注意一下，这个事件模块是modules目录下的模块的管理器。

最主要的是ngx_event_process_init函数，这个函数在初始化的时候会被调用，这个函数里又初始化了一个类型是NGX_EVENT_MODULE的模块
主要代码：
```C
    for (m = 0; cycle->modules[m]; m++) {
        if (cycle->modules[m]->type != NGX_EVENT_MODULE) {
            continue;
        }

        if (cycle->modules[m]->ctx_index != ecf->use) {
            continue;
        }

        module = cycle->modules[m]->ctx;

        if (module->actions.init(cycle, ngx_timer_resolution) != NGX_OK) {
            /* fatal */
            exit(2);
        }

        break;
    }
```
这段代码的作用就是找到第一个文件描述符的io模型，然后启动它，对所有的文件描述符进行监控和管理。

下面来看一下事件模块的实现，使用ngx_select_module举例，看一下ngx_select_module的定义：
```C
static ngx_event_module_t  ngx_select_module_ctx = {
    &select_name,
    NULL,                                  /* create configuration */
    ngx_select_init_conf,                  /* init configuration */

    {
        ngx_select_add_event,              /* add an event */
        ngx_select_del_event,              /* delete an event */
        ngx_select_add_event,              /* enable an event */
        ngx_select_del_event,              /* disable an event */
        NULL,                              /* add an connection */
        NULL,                              /* delete an connection */
        NULL,                              /* trigger a notify */
        ngx_select_process_events,         /* process the events */
        ngx_select_init,                   /* init the events */
        ngx_select_done                    /* done the events */
    }

};

ngx_module_t  ngx_select_module = {
    NGX_MODULE_V1,
    &ngx_select_module_ctx,                /* module context */
    NULL,                                  /* module directives */
    NGX_EVENT_MODULE,                      /* module type */
    NULL,                                  /* init master */
    NULL,                                  /* init module */
    NULL,                                  /* init process */
    NULL,                                  /* init thread */
    NULL,                                  /* exit thread */
    NULL,                                  /* exit process */
    NULL,                                  /* exit master */
    NGX_MODULE_V1_PADDING
};
```
ngx_select_module模块的module context字段指向了ngx_select_module_ctx，ngx_select_module_ctx结构体里封装了select相关的函数。
结合ngx_event_process_init函数，可以看出来，在ngx_event_process_init的时候，会调用ngx_select_module_ctx结构体里的ngx_select_init函数。
再来看看ngx_select_init函数里有些什么：
```C
static ngx_int_t
ngx_select_init(ngx_cycle_t *cycle, ngx_msec_t timer)
{
    ngx_event_t  **index;

    if (event_index == NULL) {
        FD_ZERO(&master_read_fd_set);
        FD_ZERO(&master_write_fd_set);
        nevents = 0;
    }

    if (ngx_process >= NGX_PROCESS_WORKER
        || cycle->old_cycle == NULL
        || cycle->old_cycle->connection_n < cycle->connection_n)
    {
        index = ngx_alloc(sizeof(ngx_event_t *) * 2 * cycle->connection_n,
                          cycle->log);
        if (index == NULL) {
            return NGX_ERROR;
        }

        if (event_index) {
            ngx_memcpy(index, event_index, sizeof(ngx_event_t *) * nevents);
            ngx_free(event_index);
        }

        event_index = index;
    }

    ngx_io = ngx_os_io;

    ngx_event_actions = ngx_select_module_ctx.actions;

    ngx_event_flags = NGX_USE_LEVEL_EVENT;

    max_fd = -1;

    return NGX_OK;
}
```
代码不多，顾名思义，就是对调用select函数做准备，首先使用FD_ZERO宏清空了fd_set，因为在调用select的时候，需要用到fd_set。
ngx_select_module_ctx在调用process_event函数指针的时候会调用ngx_select_process_events，ngx_select_process_events会调用select函数。


