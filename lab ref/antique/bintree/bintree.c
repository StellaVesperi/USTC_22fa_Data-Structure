#include<stdio.h>
#include<stdlib.h>
#define MAXNODE 256
#define true 1
#define false 0
typedef struct {
	char data;
	int left;
	int right;
} tnode;
tnode t[MAXNODE]; // 0 is the root
int nodenum = 0;
/*
int search_comma(char* list, int from, int end) {
	int i, brackets = 0;
	for (i=from; i<end; i++) {
		if (list[i] == ',' && brackets == 0)
			return i;
		if (list[i] == '(')
			brackets++;
		else if (list[i] == ')')
			brackets--;
	}
	return -1; // not found
}

int transform(char* list, int* node, int from, int end) {
	int comma;
	if (from+1 == end) { // () implies empty list
		if (list[from] == '(' && list[end] == ')') {
			*node = -1;
			return true;
		}
		else	return false;
	}
	if (list[from+1] != '(' || list[end] != ')')
		return false;
	*node = nodenum++;
	t[*node].data = list[from];
	comma = search_comma(list, from+2, end-1);
	if (comma == -1)
		return false;
	if (!transform(list, &t[*node].left, from+2, comma-1))
		return false;
	if (!transform(list, &t[*node].right, comma+1, end-1))
		return false;
	return true;
}
*/
int transform(char** list, int* node) {
	if (**list == '(') {
		(*list)++;
		if (*(*list)++ == ')')
			return true;
		return false;
	}
	*node = nodenum++;
	t[*node].data = *(*list)++;
	t[*node].left = -1;
	t[*node].right = -1;
	if (**list == '(') {
		(*list)++;
		if (!transform(list, &t[*node].left))
			return false;
		if (*(*list)++ != ',') return false;
		if (!transform(list, &t[*node].right))
			return false;
		if (*(*list)++ != ')') return false;
	}
	return true; // not including subtree
}

void printnode() {
	int i;
	for (i=0; i<nodenum; i++)
		printf("Node[%d]: %c left[%d] right[%d]\n",i,t[i].data,t[i].left,t[i].right);
}

void input() {
	char list[1000]; // max list length
	char *pt = list;
	char **p = &pt;
	int root = 0;
	printf("Input list string to create a new tree. e.g. a(b((),f),c(d,e))\n");
	scanf("%s", list);
	nodenum = 0;
	if (transform(p, &root))
		printf("Input list is transformed into binary tree.\n");
	else
		printf("Invalid list! Please check and retry.\n");
	printnode();
}

int maxheight(int node) {
	int lheight, rheight;
	lheight = t[node].left != -1 ? maxheight(t[node].left) : 0;
	rheight = t[node].right != -1 ? maxheight(t[node].right) : 0;
	return (lheight > rheight ? lheight : rheight) + 1;
}

void height() {
	printf("Tree height is %d\n", maxheight(0));
}

int countnode(int node) {
	int lnode, rnode;
	lnode = t[node].left != -1 ? countnode(t[node].left) : 0;
	rnode = t[node].right != -1 ? countnode(t[node].right) : 0;
	return (lnode + rnode + 1);
}

int countleaf(int node) {
	int lnode, rnode;
	lnode = t[node].left != -1 ? countleaf(t[node].left) : 0;
	rnode = t[node].right != -1 ? countleaf(t[node].right) : 0;
	if (lnode == 0 && rnode == 0) // leaf
		return 1;
	return lnode + rnode;
}

void preorder_traverse(int node) {
	printf("%c", t[node].data);
	if (t[node].left != -1)
		preorder_traverse(t[node].left);
	if (t[node].right != -1)
		preorder_traverse(t[node].right);
}

void inorder_traverse(int node) {
	if (t[node].left != -1)
		inorder_traverse(t[node].left);
	printf("%c", t[node].data);
	if (t[node].right != -1)
		inorder_traverse(t[node].right);
}

void postorder_traverse(int node) {
	if (t[node].left != -1)
		postorder_traverse(t[node].left);
	if (t[node].right != -1)
		postorder_traverse(t[node].right);
	printf("%c", t[node].data);
}

void traverse() {
	printf("Preorder traverse: ");
	preorder_traverse(0);
	printf("\nInorder traverse: ");
	inorder_traverse(0);
	printf("\nPostorder traverse: ");
	postorder_traverse(0);
	printf("\n");
}

void number() {
	printf("Total nodes: %d, leaf nodes: %d\n", countnode(0), countleaf(0));
}

int node_at_depth(int node, int depth, int maxdepth) {
	int count = 0;
	if (depth == maxdepth) {
		printf("%c", t[node].data);
		return 1;
	}
	if (t[node].left != -1)
		count += node_at_depth(t[node].left, depth+1, maxdepth);
	if (t[node].right != -1)
		count += node_at_depth(t[node].right, depth+1, maxdepth);
	return count;
}

void depth_order() {
	int depth = 0;
	printf("Depth order traverse: ");
	while (1) {
		if (node_at_depth(0, 0, depth++) == 0)
			break;
	}
	printf("\n");
}

void print_list(int node) {
	printf("%c", t[node].data);
	if (t[node].left == -1 && t[node].right == -1)
		return;
	printf("(");
	if (t[node].left != -1)
		print_list(t[node].left);
	else
		printf("()");
	printf(",");
	if (t[node].right != -1)
		print_list(t[node].right);
	else
		printf("()");
	printf(")");
}

void list() {
	printf("The list form of binary tree: ");
	print_list(0);
	printf("\n");
}

void usage() {
	printf("Binary Tree Demo. (C) 2011-12-16 boj\n");
	printf("Usage:\n");
	printf("(I) Input a list to create a binary tree.\n");
	printf("(H) Calculate the height of the tree.\n");
	printf("(N) Calcalate the number of nodes.\n");
	printf("(T) Output the preorder, inorder, postorder traversal of the tree.\n");
	printf("(L) Output the depth order traversal of the tree.\n");
	printf("(S) Output the list form (string) of the tree.\n");
	printf("(U) Show this usage screen.\n");
	printf("(Q) Quit\n");
}

int main() {
	usage();
	while (1) {
		char cmd = '\n';
		printf("BinTree > ");
		while (cmd == '\n' || cmd == ' ')
			scanf("%c", &cmd);
		switch (cmd) {
			case 'i':
			case 'I': input(); break;
			case 'h':
			case 'H': height(); break;
			case 'n':
			case 'N': number(); break;
			case 't':
			case 'T': traverse(); break;
			case 'l':
			case 'L': depth_order(); break;
			case 's':
			case 'S': list(); break;
			case 'u':
			case 'U': usage(); break;
			case 'q':
			case 'Q': exit(0); break;
			default: printf("Invalid command.\n");
		}
	}
}
