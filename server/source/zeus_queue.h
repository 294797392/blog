/***********************************************************************************
 * @ file    : zeus_queue.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2022.07.23 10:06
 * @ brief   : 缓存客户端请求的队列
 ************************************************************************************/

#ifndef __ZEUS_QUEUE_H__
#define __ZEUS_QUEUE_H__

typedef struct zeus_queue_s
{
    void *ptr;
}zeus_queue;

zeus_queue *new_queue(int max_size);


#endif