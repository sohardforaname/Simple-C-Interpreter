#include "binary_tree.h"
#include "stack.h"

void* bitree_new_data(bitree* _btr, const void* _dat)
{
	if (!_btr)
		catch_exce(6);
	if (!_btr->dat_arg.assign)
	{
		void* dst = malloc(_btr->dat_arg.u_size);
		memcpy(dst, _dat, _btr->dat_arg.u_size);
		return dst;
	}
	return _btr->dat_arg.assign(_dat);
}
void* bitree_delete_data(bitree* _btr, void* _dat)
{
	if (!_btr)
		catch_exce(6);
	if (!_dat)
		return NULL;
	if (!_btr->dat_arg.destroy)
		free(_dat);
	else
		_btr->dat_arg.destroy(_dat);
	return NULL;
}
bitree* bitree_init(data_arg _dat_arg)
{
	bitree* btr = (bitree*)malloc(sizeof(bitree));
	if (!btr)
		catch_exce(6);
	*btr = { (tree_node*)malloc(sizeof(tree_node)),_dat_arg };
	*(btr->rt) = { {NULL,NULL},NULL };
	return btr;
}
tree_node* tree_node_init(bitree* _btr, const void* _dat)
{
	if (!_btr)
		catch_exce(6);
	tree_node* tr_n = (tree_node*)malloc(sizeof(tree_node));
	*tr_n = { {NULL,NULL},bitree_new_data(_btr, _dat) };
	return tr_n;
}
void bitree_clear(bitree* _btr)
{
	if (!_btr)
		catch_exce(6);
	stack* s = stack_init(null_arg);
	tree_node* cur = _btr->rt;
	stack_push_no_copy(s, cur);
	while (s->size)
	{
		cur = (tree_node*)stack_top(s);
		if (!cur->ch[0] && !cur->ch[1])
		{
			bitree_delete_data(_btr, cur->dat);
			free(cur);
			stack_pop_no_delete(s);
		}
		else
		{
			if (cur->ch[0])
			{
				vec_push_back_no_copy(s, cur->ch[0]);
				cur->ch[0] = NULL;
			}
			if (cur->ch[1])
			{
				vec_push_back_no_copy(s, cur->ch[1]);
				cur->ch[1] = NULL;
			}
		}
	}
	stack_destroy(s);
	_btr->rt = NULL;
}
void* bitree_destroy(void* _btr)
{
	bitree* btr = (bitree*)_btr;
	bitree_clear(btr);
	free(btr);
	return NULL;
}