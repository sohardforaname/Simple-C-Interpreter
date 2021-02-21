#pragma once
#include "vector.h"
//private:
const size_t vec_init_size = 16;
void* vec_new_data(vector* _vec, const void* _dat)
{
	if (!_vec->dat_arg.assign)
	{
		void* dst = malloc(_vec->dat_arg.u_size);
		memcpy(dst, _dat, _vec->dat_arg.u_size);
		return dst;
	}
	return _vec->dat_arg.assign(_dat);
}
void* vec_delete_data(vector* _vec, void* _dat)
{
	if (!_vec->dat_arg.destroy)
		free(_dat);
	else
		_vec->dat_arg.destroy(_dat);
	return NULL;
}
//public:
vector* vec_init(data_arg _dat_arg)
{
	vector* ptr = (vector*)malloc(sizeof(vector));
	*ptr = { (void**)malloc(sizeof(void*) * vec_init_size),
		0,vec_init_size,_dat_arg 
	};
	return ptr;
}
size_t vec_resize(vector* _vec, size_t _size)
{
	if (!_vec)
		catch_exce(6);
	void** tmp = (void**)realloc(_vec->buf, sizeof(void*) * _size);
	if (!tmp)
		catch_exce(6);
	_vec->buf = tmp, _vec->capacity = _size;
	return _vec->capacity;
}
void vec_check_capacity(vector* _vec)
{
	if (!_vec)
		catch_exce(6);
	if (_vec->size == _vec->capacity)
		vec_resize(_vec, _vec->capacity << 1);
}
size_t vec_push_back(vector* _vec, void* _dat)
{
	if (!_vec)
		catch_exce(6);
	if (!_dat)
		catch_exce(7);
	vec_check_capacity(_vec);
	_vec->buf[_vec->size++] = vec_new_data(_vec, _dat);
	return _vec->size;
}
size_t vec_push_back_no_copy(vector* _vec, void* _dat)
{
	if (!_vec)
		catch_exce(6);
	if (!_dat)
		catch_exce(7);
	vec_check_capacity(_vec);
	_vec->buf[_vec->size++] = _dat;
	return _vec->size;
}
size_t vec_insert(vector* _vec, size_t _pos, void* _dat)
{
	if (!_vec)
		catch_exce(6);
	if (!_dat)
		catch_exce(7);
	if(_pos < 0 && _pos >= _vec->size)
		catch_exce(0);
	vec_check_capacity(_vec);
	for (int i = _vec->size - 1; i >= _pos; --i)
		_vec->buf[i + 1] = _vec->buf[i];
	_vec->buf[_pos] = vec_new_data(_vec, _dat);
	return ++_vec->size;
}
size_t vec_pop_back(vector* _vec)
{
	if (!_vec)
		catch_exce(6);
	_vec->buf[--_vec->size] = vec_delete_data(_vec, _vec->buf[_vec->size]);
	return _vec->size;
}
size_t vec_pop_back_no_delete(vector* _vec)
{
	if (!_vec)
		catch_exce(6);
	_vec->buf[--_vec->size] = NULL;
	return _vec->size;
}
size_t vec_delete(vector* _vec, size_t _pos)
{
	if (!_vec)
		catch_exce(6);
	if (_pos < 0 && _pos >= _vec->size)
		catch_exce(0);
	_vec->buf[_pos] = vec_delete_data(_vec, _vec->buf[_pos]);
	for (int i = _pos; i < _vec->size; ++i)
		_vec->buf[i] = _vec->buf[i + 1];
	return --_vec->size;
}
void vec_clear(vector* _vec)
{
	if (!_vec)
		catch_exce(6);
	for (int i = 0; i < _vec->size; ++i)
		_vec->buf[i] = vec_delete_data(_vec, _vec->buf[i]);
}
void* vec_destroy(void* _vec)
{
	if (!_vec)
		catch_exce(6);
	vector* vec = (vector*)_vec;
	vec_clear(vec);
	free(vec->buf);
	free(vec);
	return NULL;
}
void* vec_assign(const void* _vec)
{
	if (!_vec)
		catch_exce(6);
	vector* vec = (vector*)_vec;
	vector* newvec = vec_init(vec->dat_arg);
	vec_resize(newvec, vec->capacity);
	newvec->size = vec->size;
	for (int i = 0; i < newvec->size; ++i)
		newvec->buf[i] = vec->dat_arg.assign(vec->buf[i]);
	return newvec;
}