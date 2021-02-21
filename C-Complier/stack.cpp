#include "stack.h"
void* stack_top(stack* _stk)
{
	if (!_stk)
		catch_exce(6);
	if (!_stk->size)
		catch_exce(12);
	return _stk->buf[_stk->size - 1];
}
void* stack_top_and_pop(stack* _stk)
{
	if (!_stk)
		catch_exce(6);
	if (!_stk->size)
		catch_exce(12);
	void* tmp = stack_top(_stk);
	stack_pop(_stk);
	return tmp;
}
void* stack_top_and_pop_no_delete(stack* _stk)
{
	if (!_stk)
		catch_exce(6);
	if (!_stk->size)
		catch_exce(12);
	void* tmp = stack_top(_stk);
	stack_pop_no_delete(_stk);
	return tmp;
}