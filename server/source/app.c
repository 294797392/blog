#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <libY.h>

#include "default.h"
#include "errors.h"
#include "app.h"

steak_app *app = NULL;

int steak_app_init()
{
	app = (steak_app *)calloc(1, sizeof(steak_app));
	return STEAK_ERR_OK;
}

steak_app *get_steak_app()
{
	return app;
}
