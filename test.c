#include <stdio.h>
#include "stack.h"
#include "RBTree.h"
#include <stdlib.h>
#include <time.h>

//��ֵ�ȽϺ���
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

//ǰ���������
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
		tree->rb_insert(tree, &i, &i, sizeof(int), sizeof(int));	//����ڵ�
	iterator * itr = get_iterator(tree);

	
	int i = rand() % node_num;

	printf("delete %d\n", i);
	tree->rb_delete(tree, &i);

	printf("���������");
	itr->reset(tree, itr);				//���õ�����
	while (itr->has_next(tree, itr))	//ͨ�������������������
	{
		const rb_node * node = itr->get_next(tree, itr);
		printf("%d", *(int*)(node->key));
		if (node->color == BLACK)
			printf("B\t");
		else
			printf("R\t");
	}

	printf("\n");
	printf("ǰ�������");
	preorder_traversal(tree, tree->root);
	printf("\n");

	printf("\n");

	destroy_rb(tree);

	destroy_iterator(itr);
	return 0;
}