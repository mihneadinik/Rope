#include <stdlib.h>

#include "Stack.h"
#include "utils.h"

stack_t *
st_create(unsigned int data_size)
{
	stack_t *stack = malloc(sizeof(stack_t));
	DIE(stack == NULL, "Malloc failed!\n");
	stack->list = ll_create(data_size);
	return stack;
}

unsigned int
st_get_size(stack_t *st)
{
	DIE(st == NULL, "No stack!\n");
	return st->list->size;
}

// Intoarce 1 daca stiva este goala si 0 in caz contrar.
unsigned int
st_is_empty(stack_t *st)
{
	DIE(st == NULL, "No stack!\n");
	return st->list->head == NULL;
}

void *
st_peek(stack_t *st)
{
	DIE(st == NULL, "No stack!\n");
	// in case st->list->head == NULL => no acces to data
	if (st->list->head == NULL) {
		return NULL;
	}
	return st->list->head->data;
}

// remove first element => varful stivei
void
st_pop(stack_t *st)
{
	DIE(st == NULL, "No stack!\n");
	if (st->list->head == NULL)
		return;
	ll_node_t *rmv = ll_remove_nth_node(st->list, 0);
	free(rmv->data);
	free(rmv);
}

void
st_push(stack_t *st, void *new_data)
{
	DIE(st == NULL, "No stack in st_push!\n");
	ll_add_nth_node(st->list, 0, new_data);
}

void
st_clear(stack_t *st)
{
	DIE(st == NULL, "No stack in st_clear!\n");
	while(st->list->size != 0)
		st_pop(st);
}

void
st_free(stack_t *st)
{
	DIE(st == NULL, "No stack in st_free!\n");
	st_clear(st);
	free(st->list);
	free(st);
}
