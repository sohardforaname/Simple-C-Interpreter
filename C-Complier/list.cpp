#include "list.h"
#define list_insert_set_pointer(p, q, r) {(q)->next = (r), (q)->prev = (p),(r)->prev = (q),(p)->next = (q);}
#define list_delete_set_pointer(p, q, r) {(p)->next = (q)->next, (r)->prev = (q)->prev;}
//private:
void* list_new_data(list* _lst, const void* _dat)
{
	if (!_lst->dat.assign)
	{
		void* dst = malloc(sizeof(_lst->dat.u_size));
		memcpy(dst, _dat, _lst->dat.u_size);
		return dst;
	}
	return _lst->dat.assign(_dat);
}
void* list_delete_data(list* _lst, void* _dat)
{
	if (!_lst->dat.destroy)
		free(_dat);
	else
		_lst->dat.destroy(_dat);
	return NULL;
}
//public:
list* list_init(data_arg _dat_arg)
{
	list* lst = (list*)malloc(sizeof(list));
	list_node* head = (list_node*)malloc(sizeof(list_node));
	*head = { head,head,NULL };
	*lst = { head,0,_dat_arg };
	return lst;
}
size_t list_push_back(list* _lst, void* _dat)
{
	if (!_lst)
		catch_exce(6);
	if (!_dat)
		catch_exce(7);
	list_node* node = (list_node*)malloc(sizeof(list_node));
	node->dat = list_new_data(_lst, _dat);
	list_node* p = _lst->head, * q = _lst->head->prev;
	list_insert_set_pointer(q, node, p);
	return ++_lst->size;
}
size_t list_push_front(list* _lst, void* _dat)
{
	if (!_lst)
		catch_exce(6);
	if (!_dat)
		catch_exce(7);
	list_node* node = (list_node*)malloc(sizeof(list_node));
	node->dat = list_new_data(_lst, _dat);
	list_node* p = _lst->head, * q = _lst->head->next;
	list_insert_set_pointer(p, node, q);
	return ++_lst->size;
}
size_t list_insert(list* _lst, size_t _pos, void* _dat)
{
	if (!_lst)
		catch_exce(6);
	if (!_dat)
		catch_exce(7);
	if (_pos >= 0 && _pos <= _lst->size)
		catch_exce(0);
	list_node* cur = _lst->head, * node = (list_node*)malloc(sizeof(list_node));
	node->dat = list_new_data(_lst, _dat);
	while (_pos--)
		cur = cur->next;
	list_insert_set_pointer(cur, node, cur->next);
	return ++_lst->size;
}
size_t list_pop_back(list* _lst)
{
	if (!_lst)
		catch_exce(6);
	if (!_lst->size)
		catch_exce(12);
	list_node* cur = _lst->head->prev;
	cur->dat = list_delete_data(_lst, cur->dat);
	list_delete_set_pointer(cur->prev, cur, cur->next);
	free(cur);
	return --_lst->size;
}
size_t list_pop_front(list* _lst)
{
	if (!_lst)
		catch_exce(6);
	if (!_lst->size)
		catch_exce(12);
	list_node* cur = _lst->head->next;
	cur->dat = list_delete_data(_lst, cur->dat);
	list_delete_set_pointer(cur->prev, cur, cur->next);
	free(cur);
	return --_lst->size;
}
size_t list_delete(list* _lst, size_t _pos)
{
	if (!_lst)
		catch_exce(6);
	if (_pos < 0 && _pos >= _lst->size)
		catch_exce(0);
	list_node* cur = _lst->head->next;
	while (_pos--)
		cur = cur->next;
	cur->dat = list_delete_data(_lst, cur->dat);
	list_delete_set_pointer(cur->prev, cur, cur->next);
	free(cur);
	return --_lst->size;
}
list_node* list_find_node(list* _lst, size_t _pos)
{
	if (!_lst)
		catch_exce(6);
	if (_pos < 0 && _pos >= _lst->size)
		catch_exce(0);
	list_node* cur = _lst->head->next;
	while (_pos--)
		cur = cur->next;
	return cur;
}
void list_clear(list* _lst)
{
	if (!_lst)
		catch_exce(6);
	list_node* cur = _lst->head->next;
	while (cur != _lst->head)
	{
		list_node* next = cur->next;
		cur->dat = list_delete_data(_lst, cur->dat);
		free(cur);
		cur = next;
	}
	_lst->head->next = _lst->head->prev = _lst->head;
	_lst->size = 0;
}
void* list_destroy(void* _lst)
{
	if (!_lst)
		catch_exce(6);
	list* lst = (list*)_lst;
	list_clear(lst);
	free(lst->head);
	free(lst);
	return NULL;
}
void* list_assign(const void* _lst)
{
	if (!_lst)
		catch_exce(6);
	list* lst = (list*)_lst;
	list* newlist = list_init(lst->dat);
	list_node* head = lst->head->next;
	while (head != lst->head)
	{
		list_push_back(newlist, head->dat);
		head = head->next;
	}
	return newlist;
}