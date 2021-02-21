#pragma once
#include "def.h"
#define vec_get_data(_vec, _pos, _type)  ((_type*)(_vec)->buf[(_pos)])
struct vector
{
	void** buf;
	size_t size, capacity;
	data_arg dat_arg;
};
vector* vec_init(data_arg _dat_arg);
size_t vec_resize(vector* _vec, size_t _size);
size_t vec_push_back(vector* _vec, void* _dat);
size_t vec_push_back_no_copy(vector* _vec, void* _dat);
size_t vec_insert(vector* _vec, size_t _pos, void* _dat);
size_t vec_pop_back(vector* _vec);
size_t vec_pop_back_no_delete(vector* _vec);
size_t vec_delete(vector* _vec, size_t _pos);
void vec_clear(vector* _vec);
void* vec_destroy(void* _vec);
void* vec_assign(const void* _vec);