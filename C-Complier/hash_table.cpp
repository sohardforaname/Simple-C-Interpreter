#include "hash_table.h"
const int hs_tb_init_size = 997;
/*
hash_table_node_assign
*/
void hs_tb_new_key(hs_tb* _hst, size_t _pos, const void* _key)
{
	if (!_hst->key_arg.assign)
	{
		_hst->buf[_pos].key = malloc(_hst->key_arg.u_size);
		memcpy(_hst->buf[_pos].key, _key, _hst->key_arg.u_size);
	}
	else if (_hst->key_arg.assign)
		_hst->buf[_pos].key = _hst->key_arg.assign(_key);
}
void hs_tb_new_val(hs_tb* _hst, size_t _pos, const void* _val)
{
	if (!_hst->val_arg.assign)
	{
		_hst->buf[_pos].val = malloc(_hst->val_arg.u_size);
		memcpy(_hst->buf[_pos].val, _val, _hst->val_arg.u_size);
	}
	else if (_hst->val_arg.assign)
		_hst->buf[_pos].val = _hst->val_arg.assign(_val);
}
void hs_tb_delete_key(hs_tb* _hst, size_t _pos)
{
	if (_hst->key_arg.destroy)
		_hst->buf[_pos].key = _hst->key_arg.destroy(_hst->buf[_pos].key);
	else if (!_hst->key_arg.assign)
	{
		free(_hst->buf[_pos].key);
		_hst->buf[_pos].key = NULL;
	}
}
void hs_tb_delete_val(hs_tb* _hst, size_t _pos)
{
	if (_hst->val_arg.destroy)
		_hst->buf[_pos].val = _hst->val_arg.destroy(_hst->buf[_pos].val);
	else if (!_hst->val_arg.assign)
	{
		free(_hst->buf[_pos].val);
		_hst->buf[_pos].val = NULL;
	}
}
void hs_tb_new_node(hs_tb* _hst, size_t _pos, const void* _key, const void* _val)
{
	if (!_hst)
		catch_exce(6);
	if (_key)
		hs_tb_new_key(_hst, _pos, _key);
	if (_val)
		hs_tb_new_val(_hst, _pos, _val);
	_hst->buf[_pos].st = _EXIST_;
}
void hs_tb_delete_node(hs_tb* _hst, size_t _pos)
{
	if (!_hst)
		catch_exce(6);
	hs_tb_delete_key(_hst, _pos);
	hs_tb_delete_val(_hst, _pos);
	_hst->buf[_pos].st = _EMPTY_;
}
/*
end
*/
hs_tb* hs_tb_init(data_arg _key_arg, data_arg val_arg, key_hash _key_hs)
{
	hs_tb* hst = (hs_tb*)malloc(sizeof(hs_tb));
	*hst =
	{
		(hs_tb_n*)malloc(sizeof(hs_tb_n) * hs_tb_init_size),
		0, hs_tb_init_size,
		_key_arg,val_arg,_key_hs
	};
	memset(hst->buf, 0, sizeof(hs_tb_n) * hs_tb_init_size);
	return hst;
}
#include "str.h"
hs_tb_n* const hs_tb_find_node(hs_tb* _hst, const void* _key)
{
	if (!_hst)
		catch_exce(6);
	if (!_key)
		catch_exce(7);
	unsigned long hs_val = get_hash_val(_hst, _key);
	while (_hst->buf[hs_val].st != _EMPTY_)
	{
		if (!_hst->hash.is_equ(_hst->buf[hs_val].key, _key))
		{
			if (_hst->buf[hs_val].st == _DELETE_)
				return NULL;
			else
				return &_hst->buf[hs_val];
		}
		(hs_val += 1) %= _hst->capacity;
	}
	return NULL;
}
void hs_tb_insert_set_node(hs_tb* _hst, size_t _pos, const void* _key, const void* _val)
{
	if (!_hst)
		catch_exce(6);
	if (!_key || !_val)
		catch_exce(7);
	if (_hst->buf[_pos].st == _DELETE_)
		_hst->buf[_pos].st = _EXIST_;
	else
		hs_tb_new_node(_hst, _pos, _key, _val);
}
size_t hs_tb_insert_node(hs_tb* _hst, const void* _key, const void* _val)
{
	if (!_hst)
		catch_exce(6);
	if (!_key || !_val)
		catch_exce(7);
	if (hs_tb_find_node(_hst, _key))
		return _hst->size;
	unsigned long hs_val = get_hash_val(_hst, _key);
	while (_hst->buf[hs_val].st == _EXIST_)
		(hs_val += 1) %= _hst->capacity;
	hs_tb_insert_set_node(_hst, hs_val, _key, _val);
	return ++_hst->size;
}
size_t hs_tb_set_node_delete(hs_tb* _hst, const void* _key)
{
	if (!_hst)
		catch_exce(6);
	if (!_key)
		catch_exce(7);
	hs_tb_n* cur = hs_tb_find_node(_hst, _key);
	cur->st = _DELETE_;
	return --_hst->size;
}
void hs_tb_clear(hs_tb* _hst)
{
	if (!_hst)
		catch_exce(6);
	for (int i = 0; i < hs_tb_init_size; ++i)
		if (_hst->buf[i].key && _hst->buf[i].val)
			hs_tb_delete_node(_hst, i);
	_hst->size = 0;
}
void* hs_tb_destroy(void* _hst)
{
	hs_tb* hst = (hs_tb*)_hst;
	hs_tb_clear(hst);
	free(hst->buf);
	free(hst);
	return NULL;
}
void* hs_tb_assign(const void* _hst)
{
	hs_tb* hst = (hs_tb*)_hst;
	hs_tb* new_hs_tb = (hs_tb*)malloc(sizeof(hs_tb));
	*new_hs_tb = {
		(hs_tb_n*)malloc(sizeof(hs_tb_n) * hs_tb_init_size),
		hst->size,hst->capacity,
		hst->key_arg, hst->val_arg,
		hst->hash
	};
	for (int i = 0; i < hs_tb_init_size; ++i)
		if (hst->buf[i].key && hst->buf[i].val)
			new_hs_tb->buf[i] = {
				hst->key_arg.destroy(hst->buf[i].key),
				hst->val_arg.destroy(hst->buf[i].val),
				hst->buf[i].st
		};
	return new_hs_tb;
}