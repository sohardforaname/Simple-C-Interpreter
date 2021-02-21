#include "def.h"

void* int_assign(const void* _rv)
{
    if (!_rv) {
        catch_exce(6);
    }
    int* nv = (int*)malloc(sizeof(int));
    if (!nv)
        return NULL;
    memcpy(nv, _rv, sizeof(int));
    return nv;
}

const data_arg int_arg = { sizeof(int32_t),NULL,NULL };
const data_arg long_arg = { sizeof(int64_t),NULL,NULL };
const data_arg float_arg = { sizeof(float),NULL,NULL };
const data_arg double_arg = { sizeof(double),NULL,NULL };
const data_arg ptr_arg = { sizeof(void*),NULL,NULL };
const data_arg null_arg = { 0,NULL,NULL };