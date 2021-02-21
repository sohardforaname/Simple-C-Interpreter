#pragma once
#include "tokendef.h"
#include "binary_tree.h"
typedef token ast_node;
#define ast_node_arg token_arg;
#define ast_node_clear token_clear
#define ast_node_destroy token_destroy
#define ast_node_assign token_assign
tree_node* set_ast_node(int _id, void* _dat, data_arg _dat_arg);
tree_node* set_ast_node_by_token(token* _tk);
