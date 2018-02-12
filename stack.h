#ifndef _STACK_H
#define _STACK_H

typedef struct _st_node
{
	void * data;
	struct _st_node * next;
}st_node;

typedef struct _stack stack;

struct _stack
{
	st_node *top;
	int size;
	int (*push)( stack * st, void * data, int data_size);
	void (*pop)( stack * st );
	void* (*get_top)( stack *st );
	int (*get_size)( stack *st );
	void (*clear)( stack * st );
	int (*empty)(stack * st);
};

stack * get_stack ();

void free_stack(stack * st);

#endif