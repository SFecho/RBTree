CC = gcc

all: stack RBTree test main

stack:stack.c
	$(CC) -c stack.c -g -o stack.o

RBTree: RBTree.c
	$(CC) -c RBTree.c -g -o RBTree.o
	
test: test.c 
	$(CC) -c test.c -g -o test.o
	
main: stack.o RBTree.o test.o
	$(CC)  stack.o RBTree.o test.o -g -o RBTree