#pragma once
#include <ctype.h>
#include "tabledef.h"
#include "vector.h"
#include "tokendef.h"
#include "str.h"
void auto_init();
char* filter(char* _s);
void get_a_line_and_filter(FILE* _fp, char* _buf);
long double check_num(char* _s, int* _len);
int check_id(char* _s, int* _len);
int check_ope(char* _s, int* _len);
void check_string(char* _s, int* _len);
void check_char(char* _s, int* _len);
void source_to_token(vector* _tk_list, char* _s);
void print_token(token* _tk);
void get_token_list(vector* _lst, FILE* fp);