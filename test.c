#include <stdio.h>
#include "stack.h"
#include "RBTree.h"
#include <stdlib.h>
#include <time.h>

//键值比较函数
int cmp(const void *first, const void *second)
{
	int * p = (int *)first;
	int * q = (int *)second;
	
	if( *p < *q )
		return -1;
	else if( *p == *q )
		return 0;
	return 1;
}

//前序遍历函数
void  preorder_traversal(rb_tree *tree, rb_node *node)
{
	if (node != tree->nil)
	{
		printf("%d", *(int*)(node->key));
		if (node->color == BLACK)
			printf("B\t");
		else
			printf("R\t");
		preorder_traversal(tree, node->left);
		preorder_traversal(tree, node->right);
	}
}


int main(int argc, char* argv[])
{
	
	rb_tree * tree = get_rb_tree(cmp);
	srand((unsigned)time(NULL));

	int node_num = rand() % 50;

	for (int i = 0; i < node_num; i++)
		tree->rb_insert(tree, &i, &i, sizeof(int), sizeof(int));	//插入节点
	iterator * itr = get_iterator(tree);

	
	int i = rand() % node_num;

	printf("delete %d\n", i);
	tree->rb_delete(tree, &i);

	printf("中序遍历：");
	itr->reset(tree, itr);				//重置迭代器
	while (itr->has_next(tree, itr))	//通过迭代器进行中序遍历
	{
		const rb_node * node = itr->get_next(tree, itr);
		printf("%d", *(int*)(node->key));
		if (node->color == BLACK)
			printf("B\t");
		else
			printf("R\t");
	}

	printf("\n");
	printf("前序遍历：");
	preorder_traversal(tree, tree->root);
	printf("\n");

	printf("\n");

	destroy_rb(tree);

	destroy_iterator(itr);
	return 0;
}