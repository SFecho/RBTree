#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include "stack.h"


int push( stack * this, void * data, int data_size)
{
	if(data == NULL)
		return 0;
	
	st_node * node = (st_node *)malloc(sizeof(st_node));
	
	if(node == NULL)
		return 0;
	
	unsigned char * _data = (unsigned char*)malloc(data_size);
	if(_data == NULL)
		return 0;
	
	memcpy(_data, data, data_size);
	
	node->data = _data;
	
	node->next = this->top;
	this->top = node;
	this->size ++;
	return 1;
}

void pop( stack * this )
{
	if( this->top == NULL )
		return ;
	
	st_node * p = this->top;
	
	this->top = this->top->next;
	this->size --;
	free(p->data);
	free(p);
		
}

void* get_top( stack *this )
{
	if( this->top == NULL )
		return NULL;
	return this->top->data;
}
	
int get_size( stack *this )
{
	return this->size;
}

int empty(stack * this)
{
	if( this->top == NULL )
		return 1;
	return 0;
}	
	
void clear( stack * this )
{
	st_node * p = NULL;
	
	while( this && !empty(this) )
		pop(this);
	this->size = 0;
}
	



stack * get_stack ()
{
	stack * st = ( stack *)malloc(sizeof(stack));
	st->size = 0;
	st->top = NULL;
	st->push = push;
	st->pop = pop;
	st->clear = clear;
	st->get_size = get_size;
	st->empty = empty;
	st->get_top = get_top;
}



void destory_stack(stack * st)
{
	st->clear(st);
	free(st);
}