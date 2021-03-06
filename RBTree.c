#include "RBTree.h"
#include "stack.h"
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
/**
  *左旋函数
  *
  *
  */
static void left_rotate(rb_tree * tree, rb_node * node)
{
	if(node == tree->nil )
		return ;
	rb_node * rotate_node = node->right;	//该节点的右儿子
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
 *右旋函数
 *
 *
 */
static void right_rotate(rb_tree * tree, rb_node * node)
{
	if(node == tree->nil )
		return ;
	rb_node * rotate_node = node->left;				//该节点的左儿子
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
 * 插入后修复函数，使得新的树满足红黑树的性质
 *
 */
static void insert_fix_up(rb_tree * tree, rb_node * node)
{
	rb_node * uncle = tree->nil;
	while(  node->parent->color == RED )						//判断父亲节点是否为红色
	{
		if( node->parent->parent->left == node->parent )		//如果父亲节点是爷爷节点的左儿子
		{
			uncle = node->parent->parent->right;				//得到叔叔节点的位置
			if( uncle->color == RED )							//case 1:如果叔叔节点为红色，
			{
				uncle->color = node->parent->color = BLACK;		//case 1:将父亲节点与叔叔节点涂成黑色
				uncle->parent->color = RED;						//case 1:将爷爷节点涂成红色，这样能够保证黑高不变
				node = uncle->parent;							//case 1:将当前节点调整至爷爷节点
			}
			else											
			{
				if( node->parent->right == node )				//case 2:如果当前节点是父亲节点的右儿子，即爷爷节点，父亲节点，与当前节点不在同一侧
				{
					node = node->parent;						//case 2:将当前节点调整至其父亲节点
					left_rotate(tree, node);					//case 2:对当前节点进行左旋
				}
								
				node->parent->color = BLACK;					//case 3:如果当前节点是父亲节点的左儿子，即爷爷节点，父亲节点，与当前节点在同一侧
				node->parent->parent->color = RED;				//case 3:将父亲节点涂成黑色，爷爷节点涂成红色，以便于尽可能保证黑高一定又不违反红黑树的性质
				right_rotate(tree, node->parent->parent);		//case 3:对其爷爷节点进行右旋
			}
		}
		else if( node->parent->parent->right == node->parent )	//如果父亲节点是爷爷节点的右儿子	
		{
			uncle = node->parent->parent->left;					//得到叔叔节点的位置
			if( uncle->color == RED )							//case 1:如果叔叔节点为红色，
			{
				uncle->color = node->parent->color = BLACK;		//case 1:将父亲节点与叔叔节点涂成黑色
				uncle->parent->color = RED;						//case 1:将爷爷节点涂成红色，这样能够保证黑高不变
				node = uncle->parent;							//case 1:将当前节点调整至爷爷节点
			}
			else
			{
				if( node->parent->left == node )				//case 2:如果当前节点是父亲节点的左儿子，即爷爷节点，父亲节点，与当前节点不在同一侧
				{
					node = node->parent;						//case 2:将当前节点调整至其父亲节点
					right_rotate(tree, node);					//case 2:对当前节点进行右旋
				}
				node->parent->color = BLACK;					//case 3:如果当前节点是父亲节点的右儿子，即爷爷节点，父亲节点，与当前节点在同一侧
				node->parent->parent->color = RED;				//case 3:将父亲节点涂成黑色，爷爷节点涂成红色，以便于尽可能保证黑高一定又不违反红黑树的性质
				left_rotate(tree, node->parent->parent);		//case 3:对其爷爷节点进行左旋
			}
		}			
	}
	tree->root->color = BLACK;									//将根节点涂成黑色，以便于满足红黑树的性质
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
	if (delete_node->parent != tree->nil)				//如果待删除的节点的父亲节点不为空，即该节点不为根节点
	{
		if (delete_node->parent->left == delete_node)	//如果待删节点是其父亲节点的左儿子
			delete_node->parent->left = next_node;		//将其父亲节点的左指针连接至next_node
		else
			delete_node->parent->right = next_node;		//将其父亲节点的右指针连接至next_node
	}
	else
		tree->root = next_node;							//如果根节点为待删除节点，则直接将根节点指向next_node
	next_node->parent = delete_node->parent;			//将新节点的父亲指针指向待删除的节点的父亲指针指向的位置
}


//修复删除后的红黑树
static void delete_fix_up(rb_tree * tree, rb_node * node)
{
	rb_node* brothers = tree->nil;
	while (node != tree->root && node->color == BLACK)
	{
		if (node == node->parent->left)						//当前节点是其父亲的左儿子
		{
			brothers = node->parent->right;
								
			if (RED == brothers->color)						//case 1： node的兄弟是红色的，通过
			{
				brothers->color = BLACK;					//case 1：将兄弟节点置黑
				brothers->parent->color = RED;				//case 1：父亲节点置红
				left_rotate(tree, node->parent);			//case 1：以父亲节点进行左旋
				brothers = node->parent->right;				//case 1：重置兄弟节点位置
			}												//case 1：处理完情况1之后，w.color== BLACK ， 情况就变成2 3 4 了
																		
			if (brothers->left->color == BLACK && brothers->right->color == BLACK)		//case 2： node的兄弟是黑色的，并且其儿子都是黑色的。
			{
				if (node->parent->color == RED)				//case 2：如果父亲节点为红色
				{
					node->parent->color = BLACK;			//case 2：将父亲节点置黑
					brothers->color = RED;					//case 2：兄弟节点置红
					break;									//case 2：退出循环
				}
				else
				{
					brothers->color = RED;					//case 2：兄弟节点置红，以父亲节点为新的基准点进行矫正									
					node = node->parent;					//case 2：x.p左右是平衡的，但是x.p处少了一个黑结点，所以把x.p作为新的x继续循环
					continue;								//case 2：进行下一次循环
				}
			}

			
			if (brothers->right->color == BLACK)			//case 3：兄弟节点为黑色的，左孩子为红色。（走到这一步，说明w左右不同时为黑色。）
			{
				brothers->left->color = BLACK;				//case 3：让兄弟节点的左儿子置红
				brothers->color = RED;						//case 3：兄弟节点置红
				right_rotate(tree, brothers);				//case 3：右旋
				brothers = node->parent->right;				//case 3：兄弟节点重置
			}

			

			brothers->color = node->parent->color;			//case 4： 走到这一步说明兄弟节点为黑色， 兄弟节点的左孩子为黑色， 右孩子为红色。
			node->parent->color = BLACK;					//case 4： 将父亲节点置黑
			brothers->right->color = BLACK;					//case 4： 兄弟的右儿子节点置黑
			left_rotate(tree, node->parent);				//case 4： 以父亲节点进行左旋
			node = tree->root;								//case 4： 将node置为根节点
		}
		else{												//当前节点是其父亲的右儿子
			brothers = node->parent->left;					//找到兄弟节点的位置
			//1
			if (brothers->color == RED)						//case 1：如果兄弟节点为红色
			{
				brothers->color = BLACK;					//case 1：将其兄弟节点置为黑色
				node->parent->color = RED;					//case 1：将其父亲节点置为红色
				right_rotate(tree, node->parent);			//case 1：对其父亲节点进行右旋
				brothers = node->parent->left;				//case 1：重置兄弟节点位置
			}
			//2
			if (brothers->left->color == BLACK && brothers->right->color == BLACK)	//case 2： node的兄弟是黑色的，并且其儿子都是黑色的。
			{
				if (node->parent->color == RED)				//case 2：如果其父亲节点为红色
				{													
					node->parent->color = BLACK;			//case 2：将其父亲节点置为黑色
					brothers->color = RED;					//case 2：其兄弟节点置为红色
					break;									//case 2：跳出循环
				}
				else										//case 2：其父亲节点为黑色
				{
					node->parent->color = BLACK;			//case 2：将其父亲节点染成黑色
					brothers->color = RED;					//case 2：将其兄弟节点置为红色
					node = node->parent;					//case 2：将当前结点调至父亲位置
					continue;								//进入下一次循环
				}
			}

			//3
			if (brothers->left->color == BLACK)				//case 3：如果兄弟左儿子为黑色
			{
				brothers->color = RED;						//case 3：将兄弟节点置红
				brothers->right->color = BLACK;				//case 3：右儿子置黑
				brothers = node->parent->left;				//case 3：重置兄弟节点
			}

			//4
			brothers->color = brothers->parent->color;		//case 4：将兄弟颜色置为其父亲的颜色
			node->parent->color = BLACK;					//case 4：将其父亲节点置黑
			brothers->left->color = BLACK;					//case 4：兄弟的左儿子置黑
			right_rotate(tree, node->parent);				//case 4：对其父亲节点进行右旋
			node = tree->root;								//case 4：将node置为根节点
		}
	}
	node->color = BLACK;									//将当前结点置为黑色
}

//删除红黑树
int rb_delete( rb_tree * tree, void *key )
{
	if (tree == NULL || tree->root == tree->nil)
		return 0;
	
	rb_node * delete_node = get_node(tree, key);								// 待删除节点
	
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



//初始化红黑树
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
//重置迭代器
void reset(rb_tree * tree, iterator *itr)
{
	itr->p = tree->root;
	itr->st->clear(itr->st);
}

//迭代器判断函数
int has_next( rb_tree * tree, iterator *itr )
{
	if( itr->p != tree->nil || !itr->st->empty(itr->st) )
		return 1;
	return 0;
}

//得s到红黑树的下一个节点
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


//获取迭代器
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

//销毁迭代器
void destroy_iterator(iterator *itr)
{
	destory_stack(itr->st);
	free(itr);
}

//销毁红黑树，利用树的后序遍历
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