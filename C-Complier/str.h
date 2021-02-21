#pragma once
#include "def.h"
struct string
{
	char* buf;
	size_t size;
};
extern const data_arg str_arg;
string* string_init(const char* _s);
string* string_init_by_len(const char* _s, size_t _len);
void string_reset(string* _str, const char* _s);
void string_reset_by_len(string* _str, const char* _s, size_t _len);
void string_append(string* _str, const char* _s);
void string_append_by_len(string* _str, const char* _s, size_t _len);
void string_clear(string* _str);
void* string_destroy(void* _str);
void* string_assign(const void* _str);
unsigned long string_hash(const void* _str);
int string_is_equ(const void* _s, const void* _t);