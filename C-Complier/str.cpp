#include "str.h"
const data_arg str_arg = { sizeof(string),string_assign,string_destroy };
string* string_init(const char* _s)
{
	if (!_s)
		catch_exce(7);
	size_t len = strlen(_s);
	string* str = (string*)malloc(sizeof(string));
	*str = { (char*)malloc(len + 1), len };
	memmove(str->buf, _s, len);
	str->buf[len] = 0;
	return str;
}
string* string_init_by_len(const char* _s, size_t _len)
{
	if (!_s)
		catch_exce(7);
	string* str = (string*)malloc(sizeof(string));
	*str = { (char*)malloc(_len + 1), _len };
	memmove(str->buf, _s, _len);
	str->buf[_len] = 0;
	return str;
}
void string_reset(string* _str, const char* _s)
{
	if (!_str)
		catch_exce(6);
	size_t len = strlen(_s);
	*_str = { (char*)realloc(_str->buf,len + 1), len };
	memmove(_str->buf, _s, len);
	_str->buf[len] = 0;
}
void string_reset_by_len(string* _str, const char* _s, size_t _len)
{
	if (!_str)
		catch_exce(6);
	*_str = { (char*)realloc(_str->buf,_len + 1), _len };
	memmove(_str->buf, _s, _len);
	_str->buf[_len] = 0;
}
void string_append(string* _str, const char* _s)
{
	if (!_str || !_str->buf)
		catch_exce(6);
	int len = strlen(_s);
	*_str = { (char*)realloc(_str->buf, len + _str->size + 1), len + _str->size };
	strcat(_str->buf, _s);
}
void string_append_by_len(string* _str, const char* _s, size_t _len)
{
	if (!_str || !_str->buf)
		catch_exce(6);
	*_str = { (char*)realloc(_str->buf, _len + _str->size + 1), _len + _str->size };
	memmove(_str->buf + _str->size - _len, _s, _len);
	_str->buf[_str->size] = 0;
}
void string_clear(string* _str)
{
	if (!_str)
		catch_exce(6);
	free(_str->buf);
	_str->size = 0;
}
void* string_destroy(void* _str)
{
	if (!_str)
		catch_exce(6);
	string* str = (string*)_str;
	string_clear(str);
	free(str);
	return NULL;
}
void* string_assign(const void* _str)
{
	if (!_str)
		catch_exce(6);
	string* str = (string*)_str;
	string* new_str = string_init(str->buf);
	return new_str;
}
unsigned long string_hash(const void* _str)
{
	if (!_str)
		catch_exce(6);
	char* s = ((string*)_str)->buf;
	unsigned long h = 0, g;
	while (*s)
	{
		h = (h << 4) + *(s++);
		if (g = h & 0xf0000000ul)
			h ^= g >> 24;
		h &= ~g;
	}
	return h;
}
int string_is_equ(const void* _s, const void* _t)
{
	if (!_s || !_t)
		catch_exce(6);
	string* s = (string*)_s, * t = (string*)_t;
	return s->size != t->size || memcmp(s->buf, t->buf, s->size);
}