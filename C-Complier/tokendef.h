#pragma once
#include "def.h"
struct token
{
	int id;
	void* dat;
	data_arg dat_arg;
};
extern const data_arg token_arg;
void set_token(token* _tk, int _id, void* _dat, data_arg _dat_arg);
void token_clear(token* _tk);
void* token_destroy(void* _tk);
void* token_assign(const void* _tk);
