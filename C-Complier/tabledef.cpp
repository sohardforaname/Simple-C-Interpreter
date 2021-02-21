#include "tabledef.h"
hs_tb* res_hs_tb, * ope_hs_tb;
const int offset = 32;
const char* res_lab[35] =
{
	"int",		"long",		"case",		"char",		"const",	"continue",
	"default",	"do",		"double",	"else",		"enum",		"extern",
	"float",	"for",		"goto",		"if",		"auto",		"break",
	"register", "return",	"short",	"signed",	"sizeof",	"static",
	"struct",	"switch",	"typedef",	"union",	"unsigned", "void",
	"volatile", "while"
};
const char* d_ope[35] =
{
	"+",	"+=",	"-",	"-=",	"*",	"*=",	"/",	"/=",	"%",	"%=",
	"=",	"==",	">",	">=",	"<",	"<=",	"<<",	"<<=",	">>",	">>=",
	"->",	"!=",	"&",	"&&",	"&=",	"|",	"||",	"|=",	"^",	"^=",
	"."
};
const char* s_ope[5];
const char* b_ope[15] =
{
	"(",	")",	"[",	"]",	"{",	"}",	"\'",	"\"",
	";",	","
};
const int prev_table[50] =
{
	 4,14, 4,14, 3,14, 3,14, 3,14,
	14, 7 ,6, 6, 6, 6, 5,14, 5,14,
	 2, 7, 8,11,14,10,12,14, 9,14,
	 1,20,20,20,20,20,20,20,20,20,
	20,20
};
void table_init()
{
	string* str = string_init("");
	key_hash str_hash = { string_is_equ,string_hash };
	res_hs_tb = hs_tb_init(str_arg, int_arg, str_hash);
	ope_hs_tb = hs_tb_init(str_arg, int_arg, str_hash);

	hs_tb* hs_tb_t[5] = { res_hs_tb,ope_hs_tb,ope_hs_tb,ope_hs_tb };
	const char** key_t[5] = { res_lab,d_ope,s_ope,b_ope };
	int t_size_t[5] = { res_size,d_ope_size,s_ope_size,b_ope_size };
	int cnt = 0;
	for (int j = 0; j < 4; ++j)
		for (int i = 0; i < t_size_t[j]; ++i, ++cnt)
		{
			string_reset(str, key_t[j][i]);
			hs_tb_insert_node(hs_tb_t[j], str, &cnt);
		}
	string_destroy(str);
}
void table_destroy()
{
	hs_tb_destroy(res_hs_tb);
	hs_tb_destroy(ope_hs_tb);
}
int get_prev(int _id)
{
	return prev_table[_id - offset];
}