#ifndef _RBTREE_H
#define _RBTREE_H
#include "stack.h"

typedef enum _color_type
{
	RED, BLACK
}color_type;

typedef struct _iterator iterator;
typedef struct _rb_node rb_node;
typedef struct _rb_tree rb_tree;

struct _rb_node
{
	void * key;
	void * value;
	struct _rb_node * left;
	struct _rb_node * right;
	struct _rb_node * parent;
	int color;
};

struct _rb_tree
{
	rb_node * root;
	rb_node * nil;
	
	/**
	 * first < *second <--> return -1
     * first > *second <--> return 1
     * first = *second <--> return 1
     *
     */
	int		( *rb_insert)			( rb_tree * tree, void *key, void *value, int key_size, int value_size );
	void  * (* get_value)			(rb_tree *tree, void * key);
	int 	(* rb_delete)			(rb_tree * tree, void *key);
	
	int (*cmp)(const void *first, const void *second);
};

struct _iterator
{
	struct _iterator * _this;
	rb_node * p;
	stack *st;
	int (* has_next)( rb_tree * tree, iterator *itr );
	const rb_node* const (* get_next)( rb_tree * tree, iterator *itr );
	void (* reset)(rb_tree * tree, iterator *itr );
};

rb_tree * get_rb_tree( int (*cmp)(const void *first, const void *second) );

void destroy_rb(rb_tree * tree);

iterator * get_iterator( rb_tree * tree );

void destory_iterator(iterator *itr);




#endif