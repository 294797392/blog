/***********************************************************************************
 * @ file    : Yqueue.h
 * @ author  : oheiheiheiheihei
 * @ version : 0.9
 * @ date    : 2021.12.06
 * @ brief   : 一颗多叉树
 ************************************************************************************/

#ifndef __YTREE_H__
#define __YTREE_H__

#include "Ybase.h"

#define MAX_CHILD_COUNT 1024

typedef struct Ytree_s Ytree;
typedef struct Ytreenode_s Ytreenode;

struct Ytreenode_s
{
    // 该节点保存的数据
    void *data;

    // 子节点列表
    Ytreenode *children[MAX_CHILD_COUNT];

    // 子节点数量
    int num_child;

    // 父节点
    Ytreenode *parent;
};

#ifdef __cplusplus
extern "C" {
#endif

    typedef void (*Ytree_free_func)(void *data);

	typedef int (*Ytree_foreach_action)(Ytree *tree, Ytreenode *node, void *userdata);

    /*
     * 描述：
     * 实例化一棵树
     */
    YAPI Ytree *Y_create_tree();

    YAPI Ytree *Y_create_tree2(Ytree_free_func freefunc);

    YAPI Ytreenode *Y_tree_initroot(Ytree *tree, void *data);

    /*
     * 描述：
     * 新建一个节点
     * 
     * 参数：
     * @tree：tree对象
     * @foreach_action：遍历函数
     * @userdata：用户自定义数据
     */
    YAPI Ytreenode *Y_tree_newnode(Ytreenode *parent, void *data);

    /*
     * 描述：
     * 判断该树是否是一颗空树
     */
    YAPI int Y_tree_isempty(Ytree *tree);

    /*
     * 描述：
     * 对tree进行遍历操作，使用深度优先遍历
     * 
     * 参数：
     * @tree：tree对象
     * @foreach_action：遍历函数
     * @userdata：用户自定义数据
     */
    YAPI void Y_tree_foreach(Ytree *tree, Ytree_foreach_action foreach_action, void *userdata);

    /*
     * 描述：
     * 对tree进行遍历操作，使用广度优先遍历
     * 
     * 参数：
     * @tree：tree对象
     * @foreach_action：遍历函数
     * @userdata：用户自定义数据
     */
    YAPI void Y_tree_foreach2(Ytree *tree, Ytree_foreach_action foreach_action, void *userdata);

    /*
     * 描述：
     * 删除某个节点以及子节点
     * 
     * 参数：
     * @tree：tree对象
     * @node：要删除的节点
     */
    YAPI void Y_tree_delete(Ytree *tree, Ytreenode *node);

    YAPI void Y_tree_clear(Ytree *tree);

#ifdef __cplusplus
}
#endif

#endif