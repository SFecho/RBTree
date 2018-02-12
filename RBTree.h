#ifndef _RBTREE_H
#define _RBTREE_H

typedef enum _color_type
{
	RED, BLACK
}color_type;

typedef struct _rb_node
{
	void * key;
	void * value;
	struct _rb_node * left;
	struct _rb_node * right;
	struct _rb_node * parent;
	int color;
}rb_node;


typedef struct _rb_tree
{
	rb_node * root;
	rb_node * nil;
	
	/**
	 * first < *second <--> return -1
     * first > *second <--> return 1
     * first = *second <--> return 1
     *
     */
	int (*cmp)(const void *first, const void *second);
}rb_tree;

rb_tree * init_rb_tree( int (*cmp)(const void *first, const void *second) );

int rb_insert( rb_tree * tree, void *key, void *value, int key_size, int value_size );

void inorder_traversal( rb_tree * tree , void (* display)(void *data) );

void * get_value(rb_tree *tree, void * key);

void rb_destroy(rb_tree * tree);

int rb_delete(rb_tree * tree, void *key);

#endif