#include "Yfirstinclude.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#if (defined(Y_API_WIN32))
#include <Windows.h>
#elif (defined(Y_API_UNIX))
#endif

#include "Ylog.h"
#include "Ypool.h"
#include "Ylock.h"

#define MAX_POOL_SIZE		512

struct Yobject_s
{
	Yobject *next;

	void *userdata;
};

struct Ypool_s
{
	// 当前缓冲池的大小
	int size;

	// 缓冲池最大的大小
	int max_size;

	Yobject *spool;

	Ylock lock;
};

Ypool *Y_create_pool()
{
	Ypool *yp = (Ypool *)calloc(1, sizeof(Ypool));
	yp->max_size = MAX_POOL_SIZE;
	yp->size = 0;
	Y_create_lock(yp->lock);
	return yp;
}

Yobject *Y_pool_obtain(Ypool *yp)
{
	Y_lock_lock(yp->lock);

	Yobject *yo = NULL;
	if (yp->spool != NULL)
	{
		//YLOGD("reuse Yobject");
		yo = yp->spool;
		yp->spool = yo->next;
		yo->next = NULL;
		yp->size--;
	}

	Y_lock_unlock(yp->lock);

	if (yo != NULL)
	{
		return yo;
	}
	else
	{
		//YLOGD("create new Yobject");
		return (Yobject *)calloc(1, sizeof(Yobject));
	}
}

void Y_pool_recycle(Ypool *yp, Yobject *yo)
{
	Y_lock_lock(yp->lock);

	if (yp->size < yp->max_size)
	{
		yo->next = yp->spool;
		yp->spool = yo;
		yp->size++;
	}
	else
	{
		//YLOGE("Ypool is full");
	}

	Y_lock_unlock(yp->lock);
}

void *Y_object_get_data(Yobject *yo)
{
	return yo->userdata;
}

void Y_object_set_data(Yobject *yo, void *data)
{
	yo->userdata = data;
}