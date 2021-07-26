#ifndef __ROPE__
#define __ROPE__
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

#endif  // __ROPE__
