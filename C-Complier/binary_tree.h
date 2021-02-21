#include "vector.h"
struct tree_node
{
	tree_node* ch[2];
	void* dat;
};
struct bitree
{
	tree_node* rt;
	data_arg dat_arg;
};
bitree* bitree_init(data_arg _dat_arg);
tree_node* tree_node_init(bitree* _btr, const void* _dat);
void bitree_clear(bitree* _btr);
void* bitree_destroy(void* _btr);
