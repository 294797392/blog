#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#if (defined(ENV_WIN32)) || (defined(ENV_MINGW))
#include <WinSock2.h>
#include <Windows.h>
#elif (defined(ENV_UNIX))
#endif

#include <libY.h>
#include <cJSON.h>

#include "cblog_app.h"
#include "cblog_default.h"
#include "cblog_errors.h"
#include "cblog_utils.h"
#include "cblog_socket.h"
#include "cblog_factory.h"
#include "cblog_http_event.h"
#include "cblog_http_module.h"

// 事件轮询驱动
extern eventpoll_actions eventpoll_actions_select;
eventpoll_actions *eventpoll_actions_list[] =
{
	&eventpoll_actions_select,
	NULL
};

// http handler
extern cblog_http_handler cblog_default_http_handler;

// http模块列表
extern cblog_http_module cblog_demo_http_module;
cblog_http_module *cblog_http_module_list[] =
{
	&cblog_demo_http_module,
	NULL
};



cblog_app *app_instance = NULL;

static eventpoll_actions *select_evpoll_actions(eventpoll_type_enum polltype)
{
	size_t len = sizeof(eventpoll_actions_list) / sizeof(eventpoll_actions *);

	for(size_t i = 0; i < len; i++)
	{
		eventpoll_actions *actions = eventpoll_actions_list[i];
		if(actions->type == polltype)
		{
			return actions;
		}
	}

	return NULL;
}

static void init_svchost(svchost *svc, cJSON *json_svchost)
{
	cJSON *json_bind_addr = cJSON_GetObjectItem(json_svchost, "bind_addr");
	cJSON *json_port = cJSON_GetObjectItem(json_svchost, "port");
	cJSON *json_root = cJSON_GetObjectItem(json_svchost, "root");
	cJSON *json_index_page = cJSON_GetObjectItem(json_svchost, "index_page");

	char *bind_addr = json_bind_addr->valuestring;
	int port = json_port->valueint;
	char *root = json_root->valuestring;
	char *index_page = json_index_page->valuestring;

	strncpy(svc->options.bindaddr, bind_addr, strlen(bind_addr));
	strncpy(svc->options.root, root, strlen(root));
	svc->options.port = port;
	strncpy(svc->options.index_page, index_page, strlen(index_page));
}

static int start_svchost(svchost *svc)
{
	if((svc->sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
	{
		YLOGE("create socket failed, %d", cblog_socket_error());
		return CBLOG_ERR_SYSERR;
	}

	// 设置Socket选项，当程序异常退出之后再次打开程序的时候端口还可以继续使用
	int on = 1;
	if(setsockopt(svc->sock, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
	{
		YLOGE("setsockopt failed SO_REUSEADDR, %d", cblog_socket_error());
		return CBLOG_ERR_SYSERR;
	}

	// 设置要侦听的网络接口和端口
	struct sockaddr_in bdaddr;
	bdaddr.sin_family = PF_INET;
	bdaddr.sin_port = htons(svc->options.port);
	bdaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(bdaddr.sin_zero), 0, sizeof(bdaddr.sin_zero));

	// 绑定
	if(bind(svc->sock, (struct sockaddr *)&bdaddr, sizeof(struct sockaddr)) < 0)
	{
		YLOGE("bind failed, %d", cblog_socket_error());
		return CBLOG_ERR_SYSERR;
	}

	// 监听
	if(listen(svc->sock, 5) < 0)
	{
		YLOGE("listen failed, %d", cblog_socket_error());
		return CBLOG_ERR_SYSERR;
	}

	return CBLOG_ERR_OK;
}

static void init_event_module(cblog_app *app, cJSON *json)
{
	event_module *evm = (event_module *)calloc(1, sizeof(event_module));
	if(evm == NULL)
	{
		YLOGE("create evm failed, %s", strerror(errno));
		return;
	}

	cJSON *json_type = cJSON_GetObjectItem(json, "type");
	cJSON *json_timeout = cJSON_GetObjectItem(json, "timeout");
	evm->timeout_ms = json_timeout->valueint;
	evm->options.type = json_type->valueint;
	evm->events = (cblog_event_chain *)calloc(1, sizeof(cblog_event_chain));
	evm->except_events = Y_create_queue();
	evm->actions = select_evpoll_actions(evm->options.type);
	evm->actions->initialize(evm);

	app->evm = evm;
}

static void init_http_modules(cblog_app *app)
{
	cblog_http_module_chain *chain = (cblog_http_module_chain *)calloc(1, sizeof(cblog_http_module_chain));
	if(chain == NULL)
	{
		YLOGE("create cblog_http_module_chain failed, %s", strerror(errno));
		return;
	}

	int nmodule = sizeof(cblog_http_module_list) / sizeof(cblog_http_module *);

	for(int i = 0; i < nmodule; i++)
	{
		cblog_http_module *module = cblog_http_module_list[i];
		if(module == NULL)
		{
			break;
		}

		void *ctx = module->initialize(app);
		if(ctx == NULL)
		{
			YLOGE("init %s module failed", module->name);
			continue;
		}

		module->context = ctx;

		// 模块加到链表里
		Y_chain_add(cblog_http_module, chain, module);
	}
}


int cblog_app_init(const char *config)
{
	new_object(cblog_app, app);

	char *json_content = NULL;
	uint64_t json_len = 0;
	int rc = Y_file_read_all(config, &json_content, &json_len);
	if(rc != YERR_SUCCESS)
	{
		YLOGE("config file error, %s", config);
		return CBLOG_ERR_INVALID_CONFIG;
	}

	cJSON *json = cJSON_Parse(json_content);

	// 初始化svchost列表
	cJSON *array_svchost = cJSON_GetObjectItem(json, "svc_hosts");
	int nsvchost = cJSON_GetArraySize(array_svchost);
	new_array(svchost, svchosts, nsvchost);
	for(int i = 0; i < nsvchost; i++)
	{
		cJSON *json_svchost = cJSON_GetArrayItem(array_svchost, i);
		new_object(svchost, svc);
		init_svchost(svc, json_svchost);
		svchosts[i] = svc;
	}
	app->svchosts = svchosts;
	app->nsvchost = nsvchost;

	// 初始化event_module
	cJSON *json_event = cJSON_GetObjectItem(json, "event");
	init_event_module(app, json_event);

	// 初始化HttpHandler
	app->http_handler = &cblog_default_http_handler;

	// 初始化http生命周期事件列表
	app->http_evlist = new_cblog_http_event_list();

	// 初始化http模块列表
	init_http_modules(app);

	app_instance = app;

	return CBLOG_ERR_OK;
}

int cblog_app_start()
{
	int rc = CBLOG_ERR_OK;

	// 启动svchost
	for(int i = 0; i < app_instance->nsvchost; i++)
	{
		svchost *svc = app_instance->svchosts[i];
		if((rc = start_svchost(svc)) != CBLOG_ERR_OK)
		{
			return rc;
		}

		cblog_event *evt = new_cblog_svchost_event(svc);
		event_add(app_instance->evm, evt);
	}

	return CBLOG_ERR_OK;
}

cblog_app *cblog_app_get()
{
	return app_instance;
}
