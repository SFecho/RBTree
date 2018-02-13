#include "RBTree.h"
#include "stack.h"
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
/**
  *��������
  *
  *
  */
static void left_rotate(rb_tree * tree, rb_node * node)
{
	if(node == tree->nil )
		return ;
	rb_node * rotate_node = node->right;	//�ýڵ���Ҷ���
	node->right = rotate_node->left;
	
	if( rotate_node->left != tree->nil )
		rotate_node->left->parent = node;
	
	rotate_node->left = node;
	
	if( node->parent == tree->nil )
		tree->root = rotate_node;
	else
	{
		if( node->parent->left == node )
			node->parent->left = rotate_node;
		else
			node->parent->right = rotate_node;
	}
	rotate_node->parent = node->parent;
	
	node->parent = rotate_node;
	
}

/**
 *��������
 *
 *
 */
static void right_rotate(rb_tree * tree, rb_node * node)
{
	if(node == tree->nil )
		return ;
	rb_node * rotate_node = node->left;				//�ýڵ�������
	node->left = rotate_node->right;
	
	if( rotate_node->right != tree->nil )
		rotate_node->right->parent = node;
	
	rotate_node->right = node;
	
	if( node->parent == tree->nil )
		tree->root = rotate_node;
	else
	{
		if( node->parent->left == node )
			node->parent->left = rotate_node;
		else
			node->parent->right = rotate_node;
	}
	
	rotate_node->parent = node->parent;
	
	node->parent = rotate_node;
}
/**
 *
 * ������޸�������ʹ���µ�����������������
 *
 */
static void insert_fix_up(rb_tree * tree, rb_node * node)
{
	rb_node * uncle = tree->nil;
	while(  node->parent->color == RED )						//�жϸ��׽ڵ��Ƿ�Ϊ��ɫ
	{
		if( node->parent->parent->left == node->parent )		//������׽ڵ���үү�ڵ�������
		{
			uncle = node->parent->parent->right;				//�õ�����ڵ��λ��
			if( uncle->color == RED )							//case 1:�������ڵ�Ϊ��ɫ��
			{
				uncle->color = node->parent->color = BLACK;		//case 1:�����׽ڵ�������ڵ�Ϳ�ɺ�ɫ
				uncle->parent->color = RED;						//case 1:��үү�ڵ�Ϳ�ɺ�ɫ�������ܹ���֤�ڸ߲���
				node = uncle->parent;							//case 1:����ǰ�ڵ������үү�ڵ�
			}
			else											
			{
				if( node->parent->right == node )				//case 2:�����ǰ�ڵ��Ǹ��׽ڵ���Ҷ��ӣ���үү�ڵ㣬���׽ڵ㣬�뵱ǰ�ڵ㲻��ͬһ��
				{
					node = node->parent;						//case 2:����ǰ�ڵ�������丸�׽ڵ�
					left_rotate(tree, node);					//case 2:�Ե�ǰ�ڵ��������
				}
								
				node->parent->color = BLACK;					//case 3:�����ǰ�ڵ��Ǹ��׽ڵ������ӣ���үү�ڵ㣬���׽ڵ㣬�뵱ǰ�ڵ���ͬһ��
				node->parent->parent->color = RED;				//case 3:�����׽ڵ�Ϳ�ɺ�ɫ��үү�ڵ�Ϳ�ɺ�ɫ���Ա��ھ����ܱ�֤�ڸ�һ���ֲ�Υ�������������
				right_rotate(tree, node->parent->parent);		//case 3:����үү�ڵ��������
			}
		}
		else if( node->parent->parent->right == node->parent )	//������׽ڵ���үү�ڵ���Ҷ���	
		{
			uncle = node->parent->parent->left;					//�õ�����ڵ��λ��
			if( uncle->color == RED )							//case 1:�������ڵ�Ϊ��ɫ��
			{
				uncle->color = node->parent->color = BLACK;		//case 1:�����׽ڵ�������ڵ�Ϳ�ɺ�ɫ
				uncle->parent->color = RED;						//case 1:��үү�ڵ�Ϳ�ɺ�ɫ�������ܹ���֤�ڸ߲���
				node = uncle->parent;							//case 1:����ǰ�ڵ������үү�ڵ�
			}
			else
			{
				if( node->parent->left == node )				//case 2:�����ǰ�ڵ��Ǹ��׽ڵ������ӣ���үү�ڵ㣬���׽ڵ㣬�뵱ǰ�ڵ㲻��ͬһ��
				{
					node = node->parent;						//case 2:����ǰ�ڵ�������丸�׽ڵ�
					right_rotate(tree, node);					//case 2:�Ե�ǰ�ڵ��������
				}
				node->parent->color = BLACK;					//case 3:�����ǰ�ڵ��Ǹ��׽ڵ���Ҷ��ӣ���үү�ڵ㣬���׽ڵ㣬�뵱ǰ�ڵ���ͬһ��
				node->parent->parent->color = RED;				//case 3:�����׽ڵ�Ϳ�ɺ�ɫ��үү�ڵ�Ϳ�ɺ�ɫ���Ա��ھ����ܱ�֤�ڸ�һ���ֲ�Υ�������������
				left_rotate(tree, node->parent->parent);		//case 3:����үү�ڵ��������
			}
		}			
	}
	tree->root->color = BLACK;									//�����ڵ�Ϳ�ɺ�ɫ���Ա�����������������
}

int rb_insert( rb_tree * tree, void *key, void *value, int key_size, int value_size )
{
	assert( key != NULL );
	
	unsigned char * _key = (unsigned char *)malloc(key_size);
	assert(_key != NULL );
	
	unsigned char * _value = (unsigned char *)malloc(value_size);
	assert(_value != NULL );
	
	rb_node * node = (rb_node *)malloc( sizeof(rb_node) );
	assert(node != NULL);
	
	node->key = _key;
	node->value = _value;
	
	memcpy(node->key, key, key_size);
	if( value != NULL )
		memcpy(node->value, value, value_size);
	
	rb_node *itr = tree->root;
	
	rb_node *prev = tree->nil;
	
	while( itr != tree->nil )  
	{
		prev = itr;
		
		if( tree->cmp(node->key, itr->key) == -1 )
			itr = itr->left;
		else if( tree->cmp(node->key, itr->key) == 0 )
		{
			free(node->value);
			free(node->key);
			free(node);
			return 0;
		}
		else
			itr = itr->right;
	}
	
	if( prev == tree->nil )
		tree->root = node;
	else
		
	{if( tree->cmp(node->key, prev->key) == -1 )
			prev->left = node;
		else
			prev->right = node;
	}
	
	node->left = node->right = tree->nil;
	node->parent = prev;
	node->color = RED;
	
	insert_fix_up(tree, node);
	
	return 1;
}

void * get_value(rb_tree *tree, void * key)
{
	rb_node *p = tree->root;
	while (p != tree->nil)
	{
		if (tree->cmp(key, p->key) == -1)
			p = p->left;
		else if (tree->cmp(key, p->key) == 0)
			return p->value;
		else
			p = p->right;
	}
	return NULL;
}

static rb_node * get_node(rb_tree *tree, void * key)
{
	rb_node *p = tree->root;
	while (p != tree->nil)
	{
		if ( tree->cmp(key, p->key) == -1)
			p = p->left;
		else if ( tree->cmp(key, p->key) == 0 )
			return p;
		else
			p = p->right;
	}
	return p;
}

static void transplant(rb_tree *tree, rb_node * delete_node, rb_node * next_node)
{
	if ( delete_node == tree->nil)
		return ;
	if (delete_node->parent != tree->nil)				//�����ɾ���Ľڵ�ĸ��׽ڵ㲻Ϊ�գ����ýڵ㲻Ϊ���ڵ�
	{
		if (delete_node->parent->left == delete_node)	//�����ɾ�ڵ����丸�׽ڵ�������
			delete_node->parent->left = next_node;		//���丸�׽ڵ����ָ��������next_node
		else
			delete_node->parent->right = next_node;		//���丸�׽ڵ����ָ��������next_node
	}
	else
		tree->root = next_node;							//������ڵ�Ϊ��ɾ���ڵ㣬��ֱ�ӽ����ڵ�ָ��next_node
	next_node->parent = delete_node->parent;			//���½ڵ�ĸ���ָ��ָ���ɾ���Ľڵ�ĸ���ָ��ָ���λ��
}

static void delete_fix_up(rb_tree * tree, rb_node * node)
{
	while (node != tree->root && node->color == BLACK){
		if (node == node->parent->left){
			rb_node* w = node->parent->right;

			//���1 node���ֵ��Ǻ�ɫ�ģ�ͨ��
			if (RED == w->color){
				w->color = BLACK;
				w->parent->color = RED;
				left_rotate(tree, node->parent);
				w = node->parent->right;
			}//���������1֮��w.color== BLACK �� ����ͱ��2 3 4 ��

			//���2 node���ֵ��Ǻ�ɫ�ģ���������Ӷ��Ǻ�ɫ�ġ�
			if (w->left->color == BLACK && w->right->color == BLACK){
				if (node->parent->color == RED){
					node->parent->color = BLACK;
					w->color = RED;

					break;
				}
				else{
					w->color = RED;
					node = node->parent;//node.p������ƽ��ģ�����node.p������һ���ڽ�㣬���԰�node.p��Ϊ�µ�node����ѭ��
					continue;
				}
			}

			//���3 wΪ��ɫ�ģ�����Ϊ��ɫ�����ߵ���һ����˵��w���Ҳ�ͬʱΪ��ɫ����
			if (w->right->color == BLACK){
				w->left->color = BLACK;
				w->color = RED;
				right_rotate(tree, w);
				w = node->parent->right;
			}//������֮�󣬱�����4

			//���4 �ߵ���һ��˵��wΪ��ɫ�� w������Ϊ��ɫ�� �Һ���Ϊ��ɫ��

			w->color = node->parent->color;
			node->parent->color = BLACK;
			w->right->color = BLACK;
			left_rotate(tree, node->parent);
			node = tree->root;
		}
		else{
			rb_node* w = node->parent->left;
			//1
			if (w->color == RED){
				w->color = BLACK;
				node->parent->color = RED;
				right_rotate(tree, node->parent);
				w = node->parent->left;
			}
			//2
			if (w->left->color == BLACK && w->right->color == BLACK){
				if (node->parent->color == RED){
					node->parent->color = BLACK;
					w->color = RED;
					break;
				}
				else{
					node->parent->color = BLACK;
					w->color = RED;
					node = node->parent;
					continue;
				}
			}

			//3
			if (w->left->color == BLACK){
				w->color = RED;
				w->right->color = BLACK;
				w = node->parent->left;
			}

			//4
			w->color = w->parent->color;
			node->parent->color = BLACK;
			w->left->color = BLACK;
			right_rotate(tree, node->parent);
			node = tree->root;
		}
	}
}

int rb_delete( rb_tree * tree, void *key )
{
	if (tree == NULL || tree->root == tree->nil)
		return 0;
	
	rb_node * delete_node = get_node(tree, key);								// ��ɾ���ڵ�
	
	if (delete_node == tree->nil)
		return 0;

	rb_node *min_node = delete_node;
	rb_node * fix_node = tree->nil;

	if (delete_node->left != tree->nil && delete_node->right != tree->nil)
	{
		min_node = delete_node->right;
		while (min_node != tree->nil && min_node->left != tree->nil)
			min_node = min_node->left;

		if (delete_node->key)
			free(delete_node->key);
		if (delete_node->value)
			free(delete_node->value);

		delete_node->key = min_node->key;
		delete_node->value = min_node->value;	
		transplant(tree, min_node, min_node->right);
		delete_node = min_node;
	}

	if (delete_node->left == tree->nil)
	{
		fix_node = delete_node->right;
		transplant(tree, delete_node, delete_node->right);
	}
	else if (delete_node->right == tree->nil)
	{
		fix_node = delete_node->left;
		transplant(tree, delete_node, delete_node->left);
	}
	free(delete_node);
	if ( delete_node->color == BLACK )
		delete_fix_up(tree, fix_node);
	return 1;
}



rb_tree * get_rb_tree( int (*cmp)(const void *first, const void *second) )
{
	rb_tree * tree = (rb_tree *)malloc( sizeof(rb_tree) );
	assert(tree != NULL );
	
	rb_node *node = (rb_node *)malloc( sizeof(rb_node) ); 
	node->color = BLACK;
	node->left = node->right = node->parent = node;
	
	node->key = node->value = NULL;
	
	tree->root = tree->nil = node;
	
	tree->cmp = cmp;	
	tree->rb_insert = rb_insert;
	tree->get_value = get_value;
	tree->rb_delete = rb_delete;
	return tree;
}



int has_next( rb_tree * tree, iterator *itr )
{
	if( itr->p != tree->nil || !itr->st->empty(itr->st) )
		return 1;
	itr->p = tree->root;
	return 0;
}

const rb_node* const get_next( rb_tree * tree, iterator *itr )
{
	rb_node * node = tree->nil;
	rb_node ** st_p = &itr->p;
	while( itr->p != tree->nil )
	{
		itr->st->push(itr->st, st_p, sizeof(rb_node*) );
		itr->p = itr->p->left;
	}
	
	if( !itr->st->empty(itr->st) )
	{
		itr->p = *(rb_node **)itr->st->get_top(itr->st);
		node = itr->p;
		itr->p = itr->p->right;
		itr->st->pop(itr->st);
	}
	return node;
}

void reset( rb_tree * tree, iterator *itr )
{
	itr->p = tree->root;
	itr->st->clear(itr->st);
}

iterator * get_iterator( rb_tree * tree )
{
	iterator * itr = (iterator *)malloc(sizeof(iterator));
	itr->p = tree->root;
	itr->st = get_stack();
	itr->has_next = has_next;
	itr->get_next = get_next;
	itr->reset = reset;
	return itr;
}

void destory_iterator(iterator *itr)
{
	destory_stack(itr->st);
	free(itr);
}


/*
void destroy_rb(rb_tree * tree)
{
	iterator * itr = get_iterator(tree);

	while( has_next(tree, itr) )
	{
		const rb_node *node = get_next(tree, itr);
		rb_delete(tree, node->key);
		reset(tree, itr);
	}
	free(tree->nil);
	free(tree);
	free(itr);
}
	*/
void destroy_rb(rb_tree * tree)
{
	stack * st = get_stack();
	rb_node **current = &tree->root;
	rb_node **temp = NULL;
	rb_node * st_top = tree->nil;
	
	if( *current != tree->nil )
		st->push(st, current, sizeof(rb_node *) );
	while( !st->empty(st) )
	{
		st_top = *(rb_node **)st->get_top(st);
		if( st_top->left != *current && st_top->right != *current)
		{
			current = (rb_node **)st->get_top(st);
			while( (*current) != tree->nil )
			{
				if( (*current)->left != tree->nil )
				{
					if( (*current)->right != tree->nil)
					{
						temp = &(*current)->right;
						st->push(st, temp, sizeof(rb_node *) );
					}
					temp = &(*current)->left;
					st->push(st, temp, sizeof(rb_node *) );
					
				}
				else
				{
					temp = &(*current)->right;
					st->push(st, temp, sizeof(rb_node *) );
				}
				current = (rb_node **)st->get_top(st);
			}
			st->pop(st);
		}
		
		current = (rb_node **)st->get_top(st);

		st->pop(st);
		if( (*current)->key != NULL )
			free((*current)->key);
		if( (*current)->value != NULL )	
			free((*current)->value);
		free(*current);
	}
	destory_stack(st);
	free(tree->nil);
	free(tree);
}



