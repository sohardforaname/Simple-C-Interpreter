#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "exception.h"
#define get_data(_ptr,_type) ((_type*)(_ptr))
struct data_arg
{
	size_t u_size;
	void* (*assign)(const void* _src);
	void* (*destroy)(void* _dat);
};
extern const data_arg int_arg;
extern const data_arg long_arg;
extern const data_arg float_arg;
extern const data_arg double_arg;
extern const data_arg ptr_arg;
extern const data_arg null_arg;