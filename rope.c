/* Madalina Zanficu && Mihnea Dinica 313CA */
// #include "rope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "utils.h"

#define EMPTY ""



typedef struct RopeNode RopeNode;

struct RopeNode {
    RopeNode* left;
    RopeNode* right;
    const char* str;
    int weight;
};

typedef struct RopeTree RopeTree;

struct RopeTree {
    RopeNode* root;
};

typedef struct SplitPair SplitPair;

struct SplitPair {
    RopeNode* left;
    RopeNode* right;
};

// STACK STRUCTURES

typedef struct stack_t stack_t;

struct stack_t
{
	struct linked_list_t *list;
};

// LINKEDLIST STRUCTURES

typedef struct ll_node_t ll_node_t;

struct ll_node_t
{
	void* data;
	ll_node_t* next;
};

typedef struct linked_list_t linked_list_t;

struct linked_list_t
{
	ll_node_t* head;
	unsigned int data_size;
	int size;
};

RopeNode* makeRopeNode(const char* str);

RopeTree* makeRopeTree(RopeNode* root);

void printRopeNode(RopeNode* rn);

void printRopeTree(RopeTree* rt);

void debugRopeNode(RopeNode* rn, int indent);

int getTotalWeight(RopeNode* rt);

char* empty_creator();

char* string_copy(const char* str);

RopeTree* concat(RopeTree* rt1, RopeTree* rt2);

char __indexRope(RopeNode *rn, int idx);

char indexRope(RopeTree* rt, int idx);

char* search(RopeTree* rt, int start, int end);

RopeNode *concat_nodes(RopeNode *rn1, RopeNode *rn2);

void __splitRope(RopeNode* rn, int idx, stack_t **st, RopeNode** new_root);

SplitPair split(RopeTree* rt, int idx);

RopeTree* insert(RopeTree* rt, int idx, const char* str);

RopeTree* delete(RopeTree* rt, int start, int len);

// LINKEDLIST STUFF

linked_list_t* ll_create(unsigned int data_size);

void ll_add_nth_node(linked_list_t* list, int n, const void* data);

ll_node_t* ll_remove_nth_node(linked_list_t* list, int n);

// STACK STUFF

stack_t* st_create(unsigned int data_size);

unsigned int st_get_size(stack_t *st);

void * st_peek(stack_t *st);

void st_pop(stack_t *st);

void st_push(stack_t *st, void *new_data);

void st_clear(stack_t *st);

void st_free(stack_t *st);



RopeNode* makeRopeNode(const char* str)
{
	RopeNode *new = malloc(sizeof(RopeNode));
	DIE(new == NULL, "Failed in makeRopeNode");

	new->left = NULL;
	new->right = NULL;

	new->weight = strlen(str);
	new->str = str;


	return new;
}

RopeTree* makeRopeTree(RopeNode* root)
{
	DIE(root == NULL, "Error in makeRopeTree");
	RopeTree *tree = malloc(sizeof(RopeTree));

	DIE(tree == NULL, "Error for tree in makeRopeTree");
	tree->root = root;

	return tree;
}


void printRopeNode(RopeNode* rn)
{
	if (!rn)
		return;

	if (!(rn->left) && !(rn->right)) {
		printf("%s", rn->str);
		return;
	}

	printRopeNode(rn->left);
	printRopeNode(rn->right);
}

void printRopeTree(RopeTree* rt)
{
	if (rt && rt->root) {
		printRopeNode(rt->root);
		printf("%s", "\n");
	}
}

void debugRopeNode(RopeNode* rn, int indent)
{
	if (!rn)
		return;

	for (int i = 0; i < indent; ++i)
		printf("%s", " ");

	if (!strcmp(rn->str, EMPTY))
		printf("# %d\n", rn->weight);
	else
		printf("%s %d\n", rn->str, rn->weight);

	debugRopeNode(rn->left, indent+2);
	debugRopeNode(rn->right, indent+2);
}



int getTotalWeight(RopeNode* rt)
{
	if (!rt)
		return 0;

	return rt->weight + getTotalWeight(rt->right);
}

char* empty_creator()
{
	char *str = malloc(1);
	str[0] = '\0';
	return str;
}

char* string_copy(const char* str)
{
	// allocate memory for the copy
	char *str_copy = malloc(strlen(str) + 1);

	// copy byte by byte until the end of string
	while(*str != '\0') {
		*str_copy = *str;
		str_copy++;
		str++;
	}

	// add the terminator
	*str_copy = '\0';
	return str_copy;
}

RopeTree* concat(RopeTree* rt1, RopeTree* rt2)
{
	// verify that we have valid trees
	DIE(rt1 == NULL || rt2 == NULL, "Error in concat");

	// create a new root node
	char *empty = empty_creator();
	RopeNode *new_root = makeRopeNode(empty);
	int left_weight = getTotalWeight(rt1->root);

	// initialise root's fields
	new_root->weight = left_weight;
	new_root->left = rt1->root;
	new_root->right = rt2->root;

	// create the actual tree
	RopeTree *new_RopeTree = makeRopeTree(new_root);
	return new_RopeTree;
}

char __indexRope(RopeNode *rn, int idx)
{
	// idx is greater than te left subtree's weight
	// => check the right subtree
	if (rn->weight <= idx && rn->right != NULL) {
		return __indexRope(rn->right, idx - rn->weight);
	} else {
		// idx is smaller than the left subtree's weight
		// => check the left subtree
		if (rn->left != NULL) {
			return __indexRope(rn->left, idx);
		} else {
			// we are on a leaf
			return rn->str[idx];
		}
	}
}

char indexRope(RopeTree* rt, int idx)
{
	// verify that we have a valid tree
	DIE(rt == NULL, "Error in indexRope");

	return __indexRope(rt->root, idx);
}

char* search(RopeTree* rt, int start, int end)
{
	// verify that we have a valid tree
	DIE(rt == NULL, "Error in search");

	// allocating memory for the string that will be returned
	char *s = malloc(end - start + 1);

	// used the indexRope function to find every character in the interval
	int j = 0;
	for (int i = start; i < end; ++i) {
		// ad it to a string
		s[j++] = indexRope(rt, i);
	}

	// add the terminator
	s[j] = '\0';
	return s;
}

RopeNode *concat_nodes(RopeNode *rn1, RopeNode *rn2)
{
	// create a new parent node and allocate its fields
	// each of the nodes given as parameters is a son
	char *empty = empty_creator();
	RopeNode *new = makeRopeNode(empty);
	int left_weight = getTotalWeight(rn1);

	new->left = rn1;
	new->right = rn2;
	new->weight = left_weight;

	return new;
}

void __splitRope(RopeNode* rn, int idx, stack_t **st, RopeNode** new_root)
{
	// if idx is equal to the node's weight we split the tree into
	// a left side and a right side from the current node
	if (idx == rn->weight) {
		if (rn->right != NULL) {
			// if we have a right subtree we add it to the stack
			st_push(*st, &rn->right);
		}
		
		// copy the node to the temporary left tree
		char* str_copy = string_copy(rn->str);
		*new_root = makeRopeNode(str_copy);
		(*new_root)->right = NULL;
		(*new_root)->left = rn->left;
		(*new_root)->weight = rn->weight;

		return;
	} else {
		// if idx is greater than the node's weeight we check the right
		// subtree (if we have one)
		if (idx > rn->weight && rn->right != NULL) {
			// copy the node to the temporary left tree
			char* str_copy = string_copy(rn->str);
			*new_root = makeRopeNode(str_copy);
			(*new_root)->right = rn->right;
			(*new_root)->left = rn->left;
			(*new_root)->weight = rn->weight;

			// continue searching for the split position
			__splitRope(rn->right, idx - rn->weight, st, &(*new_root)->right);
		} else {
			// if idx is smaller that the node's weight we check the left
			// subtree (if we have one)
			if (idx < rn->weight && rn->left != NULL) {
				// copy the node to the temporary left tree
				char* str_copy = string_copy(rn->str);
				*new_root = makeRopeNode(str_copy);
				(*new_root)->right = NULL;
				(*new_root)->left = rn->left;
				(*new_root)->weight = rn->weight;

				if (rn->right != NULL) {
					// if we have a right subtree we add it to the stack
					st_push(*st, &rn->right);
				}
				// continue searching for the split position
				__splitRope(rn->left, idx, st, &(*new_root)->left);

				// update the node's weight when recursivity is over
				(*new_root)->weight = getTotalWeight((*new_root)->left);
			} else {
				// we have to split the leaf into 2 new leaves

				// allocate memory for the new strings and copy their characters
				int i, j = 0;
				char *left_str = malloc(idx + 1);
				char *right_str = malloc(strlen(rn->str) - idx + 13);

				for (i = 0; i < idx; ++i)
					left_str[i] = rn->str[i];
				left_str[i] = '\0';
				while (i < (int)(strlen(rn->str))) {
					right_str[j++] = rn->str[i++];
				}
				right_str[j] = '\0';

				// create 2 new nodes from the strings defined above
				RopeNode *left_child = makeRopeNode(left_str);
				RopeNode *right_child = makeRopeNode(right_str);

				// create a new parrent node and allocate its fields
				char *empty = empty_creator();
				*new_root = makeRopeNode(empty);
				// weight is the length of the word in its left son => idx
				(*new_root)->weight = idx;
				(*new_root)->left = left_child;
				// doesn't have a right child as it will be added to the stack
				(*new_root)->right = NULL;

				st_push(*st, &right_child);

				return;
			}
		}
	}
}

SplitPair split(RopeTree* rt, int idx)
{
	// verify that we have a valid tree
	DIE(rt == NULL, "Error in split");

	// edge case => splitting for index 0
	if (idx == 0) {
		SplitPair pair;
		// create an empty node for the left subtree
		char *empty = empty_creator();
		pair.left = makeRopeNode(empty);
		// the right subtree is the actual tree
		pair.right = rt->root;
		return pair;
	}

	// edge case => splitting for max index
	if (idx == getTotalWeight(rt->root)) {
		SplitPair pair;
		// the left subtree is the actual tree
		pair.left = rt->root;
		// create an empty node for the left subtree
		char *empty = empty_creator();
		pair.right = makeRopeNode(empty);
		return pair;
	}
	// the root for the temporary tree
	RopeNode *new_root;

	// create a stack where the nodes of the right subtree will be added
	stack_t *st = st_create(sizeof(RopeNode**));
	DIE(st == NULL, "Error creating stack");
	// call the actual split function
	__splitRope((rt->root), idx, &st, &new_root);

	// create the pair that will be returned
	SplitPair pair;
	// left subtree is the temporary tree created above
	pair.left = new_root;

	// concatenating all the nodes from the stack until
	// we have a complete subtree
	while (st_get_size(st) >= 2) {
		// taking 2 nodes out at once and concatenating them
		RopeNode *node1 = *(RopeNode **)(st_peek(st));
		st_pop(st);

		RopeNode *node2 = *(RopeNode **)(st_peek(st));
		st_pop(st);

		RopeNode *node_in = concat_nodes(node1, node2);
		// then adding the newly-created node to the stack
		st_push(st, &node_in);
	}
	// now the right subtree is the only element in the stack
	pair.right = *((RopeNode **)st_peek(st));
	// stack is no longer needed
	st_free(st);

	return pair;
}

RopeTree* insert(RopeTree* rt, int idx, const char* str)
{
	DIE(rt == NULL, "Error in insert");
	// splitting the tree by the index
	SplitPair tree_sides = split(rt, idx);

	// creating the new node to be added
	RopeNode* add_node = makeRopeNode(str);

	// concatenating everything together
	RopeNode *left_middle = concat_nodes(tree_sides.left, add_node);
	RopeNode *root_out = concat_nodes(left_middle, tree_sides.right);

	RopeTree *tree_out = makeRopeTree(root_out);
	// creating the tree
	return tree_out;
}

RopeTree* delete(RopeTree* rt, int start, int len)
{
	DIE(rt == NULL, "Error in delete");

	// splitting the tree 2 times
	SplitPair tree_sides1 = split(rt, start);
	SplitPair tree_sides2 = split(rt, start + len);

	// concatenating the sides into a tree
	RopeNode *root_out = concat_nodes(tree_sides1.left, tree_sides2.right);
	// creating the tree
	RopeTree *tree_out = makeRopeTree(root_out);

	return tree_out;
}


// FINAL 10p -> complex test involving all operations

// STACK FUNCTIONS

stack_t* st_create(unsigned int data_size)
{
	stack_t *stack = malloc(sizeof(stack_t));
	DIE(stack == NULL, "Malloc failed!\n");
	stack->list = ll_create(data_size);
	return stack;
}

unsigned int st_get_size(stack_t *st)
{
	DIE(st == NULL, "No stack!\n");
	return st->list->size;
}

void* st_peek(stack_t *st)
{
	DIE(st == NULL, "No stack!\n");
	// in case st->list->head == NULL => no acces to data
	if (st->list->head == NULL) {
		return NULL;
	}
	return st->list->head->data;
}

void st_pop(stack_t *st)
{
	DIE(st == NULL, "No stack!\n");
	if (st->list->head == NULL)
		return;
	ll_node_t *rmv = ll_remove_nth_node(st->list, 0);
	free(rmv->data);
	free(rmv);
}

void st_push(stack_t *st, void *new_data)
{
	DIE(st == NULL, "No stack in st_push!\n");
	ll_add_nth_node(st->list, 0, new_data);
}

void st_clear(stack_t *st)
{
	DIE(st == NULL, "No stack in st_clear!\n");
	while(st->list->size != 0)
		st_pop(st);
}

void st_free(stack_t *st)
{
	DIE(st == NULL, "No stack in st_free!\n");
	st_clear(st);
	free(st->list);
	free(st);
}

// LINKEDLIST FUNCTIONS

linked_list_t* ll_create(unsigned int data_size)
{
	linked_list_t *list = malloc(sizeof(linked_list_t));
	DIE(list == NULL, "Memory ups");
	list->head = NULL;
	list->data_size = data_size;
	list->size = 0;
	return list;
}

void ll_add_nth_node(linked_list_t* list, int n, const void* new_data)
{
	DIE(list == NULL, "No list\n");
	ll_node_t *nod = malloc(sizeof(ll_node_t));
	DIE(nod == NULL, "Memory ups\n");
	nod->data = malloc(list->data_size);
	memcpy(nod->data, new_data, list->data_size);

	if (n >= list->size) {
		n = list->size;
	}
	// adding on the first position
	if (n == 0) {
		nod->next = list->head;
		list->head = nod;
		list->size++;

	} else {
		// adding on the n position
		ll_node_t *current;
		current = list->head;
		for (int i = 0; i < n - 1; i++) {
			current = current->next;
		}
		nod->next = current->next;
		current->next = nod;
		list->size++;
	}
}

ll_node_t* ll_remove_nth_node(linked_list_t* list, int n)
{
	DIE(list == NULL, "No list\n");
	ll_node_t *nod;
	DIE(n < 0, "Error\n");
	// only one node in the list
	if (list->size == 1) {
		nod = list->head;
		list->head = NULL;
		list->size--;
		return nod;
	}
	if (n >= list->size -1) {
		n = list->size - 1;
	}
	// removing the first node
	if (n == 0) {
		nod = list->head;
		list->head = nod->next;
		list->size--;
		return nod;

	} else {
		// removing the nth node
		ll_node_t *prev;
		nod = list->head->next;
		prev = list->head;
		for (int i = 0; i < n - 1; i++) {
			prev = nod;
			nod = nod->next;
		}
		prev->next = nod ->next;
		list->size--;
		return nod;
	}
}
