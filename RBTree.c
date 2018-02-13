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


//�޸�ɾ����ĺ����
static void delete_fix_up(rb_tree * tree, rb_node * node)
{
	rb_node* brothers = tree->nil;
	while (node != tree->root && node->color == BLACK)
	{
		if (node == node->parent->left)						//��ǰ�ڵ����丸�׵������
		{
			brothers = node->parent->right;
								
			if (RED == brothers->color)						//case 1�� node���ֵ��Ǻ�ɫ�ģ�ͨ��
			{
				brothers->color = BLACK;					//case 1�����ֵܽڵ��ú�
				brothers->parent->color = RED;				//case 1�����׽ڵ��ú�
				left_rotate(tree, node->parent);			//case 1���Ը��׽ڵ��������
				brothers = node->parent->right;				//case 1�������ֵܽڵ�λ��
			}												//case 1�����������1֮��w.color== BLACK �� ����ͱ��2 3 4 ��
																		
			if (brothers->left->color == BLACK && brothers->right->color == BLACK)		//case 2�� node���ֵ��Ǻ�ɫ�ģ���������Ӷ��Ǻ�ɫ�ġ�
			{
				if (node->parent->color == RED)				//case 2��������׽ڵ�Ϊ��ɫ
				{
					node->parent->color = BLACK;			//case 2�������׽ڵ��ú�
					brothers->color = RED;					//case 2���ֵܽڵ��ú�
					break;									//case 2���˳�ѭ��
				}
				else
				{
					brothers->color = RED;					//case 2���ֵܽڵ��ú죬�Ը��׽ڵ�Ϊ�µĻ�׼����н���									
					node = node->parent;					//case 2��x.p������ƽ��ģ�����x.p������һ���ڽ�㣬���԰�x.p��Ϊ�µ�x����ѭ��
					continue;								//case 2��������һ��ѭ��
				}
			}

			
			if (brothers->right->color == BLACK)			//case 3���ֵܽڵ�Ϊ��ɫ�ģ�����Ϊ��ɫ�����ߵ���һ����˵��w���Ҳ�ͬʱΪ��ɫ����
			{
				brothers->left->color = BLACK;				//case 3�����ֵܽڵ��������ú�
				brothers->color = RED;						//case 3���ֵܽڵ��ú�
				right_rotate(tree, brothers);				//case 3������
				brothers = node->parent->right;				//case 3���ֵܽڵ�����
			}

			

			brothers->color = node->parent->color;			//case 4�� �ߵ���һ��˵���ֵܽڵ�Ϊ��ɫ�� �ֵܽڵ������Ϊ��ɫ�� �Һ���Ϊ��ɫ��
			node->parent->color = BLACK;					//case 4�� �����׽ڵ��ú�
			brothers->right->color = BLACK;					//case 4�� �ֵܵ��Ҷ��ӽڵ��ú�
			left_rotate(tree, node->parent);				//case 4�� �Ը��׽ڵ��������
			node = tree->root;								//case 4�� ��node��Ϊ���ڵ�
		}
		else{												//��ǰ�ڵ����丸�׵��Ҷ���
			brothers = node->parent->left;					//�ҵ��ֵܽڵ��λ��
			//1
			if (brothers->color == RED)						//case 1������ֵܽڵ�Ϊ��ɫ
			{
				brothers->color = BLACK;					//case 1�������ֵܽڵ���Ϊ��ɫ
				node->parent->color = RED;					//case 1�����丸�׽ڵ���Ϊ��ɫ
				right_rotate(tree, node->parent);			//case 1�����丸�׽ڵ��������
				brothers = node->parent->left;				//case 1�������ֵܽڵ�λ��
			}
			//2
			if (brothers->left->color == BLACK && brothers->right->color == BLACK)	//case 2�� node���ֵ��Ǻ�ɫ�ģ���������Ӷ��Ǻ�ɫ�ġ�
			{
				if (node->parent->color == RED)				//case 2������丸�׽ڵ�Ϊ��ɫ
				{													
					node->parent->color = BLACK;			//case 2�����丸�׽ڵ���Ϊ��ɫ
					brothers->color = RED;					//case 2�����ֵܽڵ���Ϊ��ɫ
					break;									//case 2������ѭ��
				}
				else										//case 2���丸�׽ڵ�Ϊ��ɫ
				{
					node->parent->color = BLACK;			//case 2�����丸�׽ڵ�Ⱦ�ɺ�ɫ
					brothers->color = RED;					//case 2�������ֵܽڵ���Ϊ��ɫ
					node = node->parent;					//case 2������ǰ����������λ��
					continue;								//������һ��ѭ��
				}
			}

			//3
			if (brothers->left->color == BLACK)				//case 3������ֵ������Ϊ��ɫ
			{
				brothers->color = RED;						//case 3�����ֵܽڵ��ú�
				brothers->right->color = BLACK;				//case 3���Ҷ����ú�
				brothers = node->parent->left;				//case 3�������ֵܽڵ�
			}

			//4
			brothers->color = brothers->parent->color;		//case 4�����ֵ���ɫ��Ϊ�丸�׵���ɫ
			node->parent->color = BLACK;					//case 4�����丸�׽ڵ��ú�
			brothers->left->color = BLACK;					//case 4���ֵܵ�������ú�
			right_rotate(tree, node->parent);				//case 4�����丸�׽ڵ��������
			node = tree->root;								//case 4����node��Ϊ���ڵ�
		}
	}
	node->color = BLACK;									//����ǰ�����Ϊ��ɫ
}

//ɾ�������
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
		{
			free(delete_node->key);
			delete_node->key = NULL;
		}
		if (delete_node->value)
		{
			free(delete_node->value);
			delete_node->value = NULL;
		}

		delete_node->key = min_node->key;
		delete_node->value = min_node->value;	
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
	
	if ( delete_node->color == BLACK )
		delete_fix_up(tree, fix_node);
	free(delete_node);
	return 1;
}



//��ʼ�������
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
//���õ�����
void reset(rb_tree * tree, iterator *itr)
{
	itr->p = tree->root;
	itr->st->clear(itr->st);
}

//�������жϺ���
int has_next( rb_tree * tree, iterator *itr )
{
	if( itr->p != tree->nil || !itr->st->empty(itr->st) )
		return 1;
	return 0;
}

//��s�����������һ���ڵ�
rb_node* get_next( rb_tree * tree, iterator *itr )
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


//��ȡ������
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

//���ٵ�����
void destroy_iterator(iterator *itr)
{
	destory_stack(itr->st);
	free(itr);
}

//���ٺ�������������ĺ������
void destroy_rb(rb_tree * tree)
{
	stack * st = get_stack();

	rb_node *p = tree->root;

	rb_node *visited = tree->nil;

	do{
		while (p != tree->nil)
		{
			st->push(st, &p, sizeof(rb_node*));
			p = p->left;
		}

		if (!st->empty(st))
		{
			p = *(rb_node **)st->get_top(st);
			if (p->right != tree->nil && p->right != visited)
				p = p->right;
			else
			{
				if (p->key != NULL)
				{
					free(p->key);
					p->key = NULL;
				}
				if (p->value != NULL)
				{
					free(p->value);
					p->value = NULL;
				}
				free(p);
				visited = p;
				p = tree->nil;
				st->pop(st);
			}
		}
	} while (!st->empty(st));


	destory_stack(st);
	free(tree->nil);
	free(tree);
}