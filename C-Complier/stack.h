#pragma once
#include "vector.h"
typedef vector stack;
#define stack_init vec_init
#define stack_resize vec_resize
#define stack_push vec_push_back
#define stack_push_no_copy vec_push_back_no_copy
#define stack_pop vec_pop_back
#define stack_pop_no_delete vec_pop_back_no_delete
#define stack_clear vec_clear
#define stack_destroy vec_destroy
#define stack_assign vec_assign
void* stack_top(stack* _stk);
void* stack_top_and_pop(stack* _stk);
void* stack_top_and_pop_no_delete(stack* _stk);