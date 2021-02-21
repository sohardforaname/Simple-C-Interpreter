#pragma once
#include "hash_table.h"
#include "str.h"
enum
{
	numb = 100, name, str_n, ch_n
};
enum
{
	_int_ = 105,       _unsigned_int_,
	     _long_,      _unsigned_long_,
	_long_long_, _unsigned_long_long_,
	   _double_,        _long_double_,
	    _float_
};
enum
{
	res_size = 32, d_ope_size = 31, s_ope_size = 0,
	b_ope_size = 10, s_quote = 69, d_quote = 70
};
enum
{
	res_rng = 32, d_ope_rng = 63,
	s_ope_rng = 63, b_ope_rng = 73
};
extern hs_tb* res_hs_tb, * ope_hs_tb;
extern const char* res_lab[35];
extern const char* d_ope[35];
extern const char* s_ope[5];
extern const char* b_ope[15];
void table_init();
void table_destroy();
int get_prev(int _id);