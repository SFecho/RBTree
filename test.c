#include <stdio.h>
#include "RBTree.h"
#include "stack.h"
#include <stdlib.h>

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


void displayFunc(void *p)
{
	rb_node *node = (rb_node *)p;
	printf("%d", *(int*)(node->key) ); 
	if (node->color == BLACK)
		printf("B\t");
	else
		printf("R\t");
}

int main( int argc,char* argv[] )
{
	rb_tree * tree = init_rb_tree( cmp );

	int i = 11;
	for (i = 0; i < 7; i++ )
		rb_insert(tree, &i, &i, sizeof(int), sizeof(int));

	inorder_traversal(tree, displayFunc);
	printf("\n");
	
	for (i = 0; i < 7; i++)
		rb_delete(tree, &i);

	inorder_traversal(tree, displayFunc);
	printf("\n");
	preorder_traversal(tree, tree->root);
	printf("\n");
	
	rb_destroy(tree);


	return 0;
}