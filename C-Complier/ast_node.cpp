#include "ast_node.h"
tree_node* set_ast_node(int _id, void* _dat, data_arg _dat_arg)
{
	tree_node* node = (tree_node*)malloc(sizeof(tree_node));
	*node = { {NULL,NULL},NULL };
	ast_node* ast_n = (ast_node*)malloc(sizeof(ast_node));
	set_token(ast_n, _id, _dat, _dat_arg);
	node->dat = ast_n;
	return node;
}
tree_node* set_ast_node_by_token(token* _tk)
{
	tree_node* node = (tree_node*)malloc(sizeof(tree_node));
	*node = { {NULL,NULL},NULL };
	ast_node* ast_n = (ast_node*)ast_node_assign(_tk);
	node->dat = ast_n;
	return node;
}