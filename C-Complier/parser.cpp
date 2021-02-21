#include "parser.h"
static int cnt = 0;
void print_tree(tree_node* t)
{
	++cnt;
	if (t->dat)
	{
		print_token((token*)t->dat);
		printf("\n");
	}
	if (t->ch[0])
		print_tree(t->ch[0]);
	if (t->ch[1])
		print_tree(t->ch[1]);
}
int parse(FILE* _fp)
{
	printf("lexical analyzing start\n");
	vector* vec = vec_init({ sizeof(token),token_assign,token_destroy });
	get_token_list(vec, _fp);
	for (int i = 0; i < vec->size; ++i)
	{
		printf("%d ", i);
		print_token(vec_get_data(vec, i, token));
		printf("\n");
	}
	printf("syntax analyzing...\n");
	ast_stack_init();
	bitree* ast = build_ast(vec);
	ast_stack_destroy();
	printf("print syntax tree\n");
	print_tree(ast->rt);
	printf("end\n");
	bitree_destroy(ast);
	vec_destroy(vec);
	return 0;
}