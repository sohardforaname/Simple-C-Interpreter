#pragma once
#include "def.h"
struct list_node
{
	list_node* next;
	list_node* prev;
	void* dat;
};
struct list
{
	list_node* head;
	size_t size;
	data_arg dat;
};
#define list_get_node(_lst, _pos, _type) ((_type*)((list_find_node(_lst, _pos))->dat))
#define next_p(_ptr) (_ptr->next)
#define prev_p(_ptr) (_ptr->prev)
#define is_head(_ptr, _lst) (_ptr == _lst->head)
list* list_init(data_arg _dat_arg);
size_t list_push_back(list* _lst, void* _dat);
size_t list_push_front(list* _lst, void* _dat);
size_t list_insert(list* _lst, size_t _pos, void* _dat);
size_t list_pop_back(list* _lst);
size_t list_pop_front(list* _lst);
size_t list_delete(list* _lst, size_t _pos);
list_node* list_find_node(list* _lst, size_t _pos);
void list_clear(list* _lst);
void* list_destroy(void* _lst);
void* list_assign(const void* _lst);
