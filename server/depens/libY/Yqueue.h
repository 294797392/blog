/***********************************************************************************
 * @ file    : Yqueue.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2021.12.06
 * @ brief   : 多线程缓冲队列，先进先出，使用环形缓冲区
 * @ remark  : 
 * 该队列使用单独的线程消费元素，并提供消费者callback给调用者使用
 * 入队和出队是在不同线程进行，互不影响
 * 队列有着固定的大小，类似于环形缓冲区
 * 如果队列满了而此时还在继续入队，那么丢弃最早入队的元素，并把入队的元素从头开始插入
 * 
 * 如果你入队的元素是手动开辟的内存空间，那么你最好调用Y_queue_set_full_callback
 * 以便于当有元素溢出队列的时候进行释放内存的操作.
 * 
 * 不要直接调用Y_queue_enqueue和Y_queue_dequeue函数，Y_queue_start会自动调用该函数并通过回调的方式把element返回
 * 除非你没有调用Y_queue_start，那么请永远不要直接调用这两个函数
 ************************************************************************************/

#ifndef __YQUEUE_H__
#define __YQUEUE_H__

#include "Yfirstinclude.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Ybase.h"

typedef enum
{
	YQUEUE_STATE_IDLE,
	YQUEUE_STATE_RUNNING
}Yqueue_state;

typedef void(*Yqueue_callback)(void *userdata, void *element);
typedef void(*Yqueue_full_callback)(void *element, void *userdata);
typedef struct Yqueue_s Yqueue;

#ifdef __cplusplus
extern "C" {
#endif

    /*
     * 描述：
     * 创建一个缓冲队列
	 * 该队列使用单独的线程消费元素，并提供消费者callback给调用者使用
	 * 入队和出队是在不同线程进行，互不影响
	 * 队列有着固定的大小，类似于环形缓冲区
	 * 如果队列满了而此时还在继续入队，那么丢弃最早入队的元素，并把入队的元素从头开始插入
     * 
     * 参数：
     * @userdata：用户自定义对象，在回调里会当成参数回调给用户
     * 
     * 返回值：
     * Yqueue对象
	 * 
	 * 注意：
	 * 如果你入队的元素是手动开辟的内存空间，那么你最好调用Y_queue_set_full_callback
	 * 以便于当有元素溢出队列的时候进行释放内存的操作.
	 * 
	 * 不要直接调用Y_queue_enqueue和Y_queue_dequeue函数，Y_queue_start会自动调用该函数并通过回调的方式把element返回
	 * 除非你没有调用Y_queue_start，那么请永远不要直接调用这两个函数
     */
	YAPI Yqueue *Y_create_queue(void *userdata);

    /*
     * 描述：
     * 删除队列并释放所占用的资源
     * 
     * 参数：
     * @q：要删除的队列
     */
	YAPI void Y_delete_queue(Yqueue *q);

    /*
     * 描述：
     * 创建一个线程并开始消费元素
     * 
     * 参数：
     * @q：要操作的队列对象
	 * @callback：每消费了一个元素，就会通过该回调回调给用户，用户可以在该回调里做操作
     */
	YAPI void Y_queue_start(Yqueue *queue, Yqueue_callback callback);
	
	/*
     * 描述：
     * 设置当缓冲队列满了之后，溢出的元素如何处理的回调
     * 
     * 参数：
     * @yq：要设置的队列对象
	 * @callback：当缓冲区溢出的时候，回调溢出的元素
     */
	YAPI void Y_queue_set_full_callback(Yqueue *yq, Yqueue_full_callback callback);

	/*
     * 描述：
     * 入队函数
     * 
     * 参数：
     * @q：要入队的队列对象
	 * @element：要入队的元素
     */
	YAPI void Y_queue_enqueue(Yqueue *queue, void *element);

	/*
     * 描述：
     * 出队函数
     * 
     * 参数：
     * @q：要出队的对象
	 * 
	 * 返回值：
	 * 出队的对象
     */
	YAPI void *Y_queue_dequeue(Yqueue *queue);
	
	/*
     * 描述：
     * 获取当前队列的大小
     */
	YAPI int Y_queue_size(Yqueue *q);

#ifdef __cplusplus
extern "C" {
#endif

#endif

