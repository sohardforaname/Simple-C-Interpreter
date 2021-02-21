#pragma once
#include "stack.h"
#include "tabledef.h"
#include "ast_node.h"
#define ast_get_token_id(_ptr) ((ast_node*)((tree_node*)_ptr)->dat)->id
enum stmt_list
{
	if_stmt = 125,	/*if���*/	else_stmt,
	for_stmt,	/*for���*/		while_stmt,	/*while���*/	
	ptr_decl,					ptr_stmt,	/*���ʽ*/		
	func_decl,					func_stmt,					
	sen_beg,					bloc_stmt,					
	expr_stmt,					decl_stmt,					
	arg_list
};
void ast_stack_init();
void ast_stack_destroy();
bitree* build_ast(const vector* _vec);