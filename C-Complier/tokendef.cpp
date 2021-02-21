#include "tokendef.h"
const data_arg token_arg = { sizeof(token),token_assign,token_destroy };
void set_token(token* _tk, int _id, void* _dat, data_arg _dat_arg)
{
	if (!_tk)
		catch_exce(6);
	_tk->id = _id;
	_tk->dat_arg = _dat_arg;
	if (_dat)
	{
		if (!_dat_arg.assign)
		{
			_tk->dat = malloc(_dat_arg.u_size);
			memcpy(_tk->dat, _dat, _dat_arg.u_size);
		}
		else
			_tk->dat = _dat_arg.assign(_dat);
	}
	else
		_tk->dat = NULL;
}
void token_clear(token* _tk)
{
	if (!_tk)
		catch_exce(6);
	if (_tk->dat)
	{
		if (_tk->dat_arg.destroy)
			_tk->dat = _tk->dat_arg.destroy(_tk->dat);
		else
			free(_tk->dat);
	}
	_tk->id = 0;
	_tk->dat_arg = { 0,0,0 };
}
void* token_destroy(void* _tk)
{
	if (!_tk)
		catch_exce(6);
	token* tk = (token*)_tk;
	token_clear(tk);
	free(tk);
	return NULL;
}
void* token_assign(const void* _tk)
{
	if (!_tk)
		catch_exce(6);
	token* tk = (token*)_tk;
	token* new_tk = (token*)malloc(sizeof(token));
	set_token(new_tk, tk->id, tk->dat, tk->dat_arg);
	return new_tk;
}
