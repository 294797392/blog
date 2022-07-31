#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "zeus_log.h"
#include "zeus_config.h"
#include "zeus_event_driver.h"
#include "zeus_os.h"

extern struct zeus_event_driver_operation_s zeus_event_driver_select;
static struct zeus_event_driver_operation_s *zeus_event_driver_list[] =
{
	&zeus_event_driver_select,
};

zeus_event_driver *new_event_driver(zeus_config *config)
{
	zeus_event_driver *driver = (zeus_event_driver *)zeus_calloc(1, sizeof(zeus_event_driver));

	// 根据mode找到对应的IO模型
	int drivers = sizeof(zeus_event_driver_list) / sizeof(struct zeus_event_driver_operation_s *);
	for(int i = 0; i < drivers; i++)
	{
		zeus_event_driver_operation *operations = zeus_event_driver_list[i];
		if(operations->type == config->event_driver_type)
		{
			driver->operations = operations;
			break;
		}
	}

	if(driver->operations == NULL)
	{
		zloge(ZTEXT("find event_driver_operations failed, mode = %d"), config->event_driver_type);
		return NULL;
	}

	// 初始化
	driver->context = driver->operations->initialize(driver);
	if(driver->context == NULL)
	{
		zloge(ZTEXT("initialize event driver operations failed"));
		return NULL;
	}

	return driver;
}

void free_event_driver(zeus_event_driver *drv)
{
	free(drv);
}

int zeus_event_driver_add_event(zeus_event_driver *driver, zeus_event *evt)
{}

int zeus_event_driver_remove_event(zeus_event_driver *driver, zeus_event *evt)
{}

