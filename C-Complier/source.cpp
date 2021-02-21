#include "source.h"
#include <math.h>

struct get_num_automa
{
	int attr[128];
	const int trans_table[12][10] =
	{
		 0, 1, 2, 2, 1,-1,-1,11,-1,-1,//0
	   	 0, 4, 4,-1,-1,10, 6,11, 7, 8,//1
		 0, 2, 2, 2,-1,-1, 6,11, 7, 8,//2
		 0, 3, 3, 3,-1,-1, 6,-1,-1, 8,//3
		 0, 4, 4,-1,-1,-1,-1,-1, 7, 8,//4
		 0, 5, 5, 5, 5,-1, 5,-1, 7, 8,//5
		 0, 3, 3, 3,-1,-1,-1,-1,-1, 8,//6
		 0,-1,-1,-1,-1,-1,-1,-1,-1, 8,//7
		 0,-1,-1,-1,-1,-1,-1,-1,-1, 0,//8
		 0,-1,-1,-1,-1,-1,-1,-1,-1,-1,//9
		-1, 5, 5, 5, 5,-1, 5,-1,-1,-1,//10
		-1, 3, 3, 3,-1,-1,-1,-1,-1,-1,//11
	};
}am;

void auto_init()
{
	am.attr['0'] = 1;
	for (int i = 1; i < 8; ++i)
		am.attr[i + '0'] = 2;//数字字符1-7
	for (int i = 8; i < 9; ++i)
		am.attr[i + '0'] = 3;//数字字符8-9
	for (int i = 0; i < 6; ++i)
		am.attr[i + 'a'] = am.attr[i + 'A'] = 4;//十六进制字符
	am.attr['x'] = am.attr['X'] = 5;
	am.attr['e'] = am.attr['E'] = 6;
	am.attr['.'] = 7;
	am.attr['u'] = am.attr['U'] = 8;
	am.attr['l'] = am.attr['L'] = 9;
}

char* filter(char* _s)
{
	static int isnotes = 0;
	char* p = _s;
	while (*p)
	{
		if (!isnotes)
		{
			if (*p == '\\' && *(p + 1) == '\\')
			{
				*p = 0;
				return _s;
			}
			else if (*p == '\\' && *(p + 1) == '*')
				isnotes = 1;
			p += 2;
		}
		else if (isnotes && *p == '*' && *(p + 1) == '\\')
		{
			isnotes = 0;
			return p + 2;
		}
	}
	return _s;
}

void get_a_line_and_filter(FILE* _fp, char* _buf)//获取一行
{
	char* beg = _buf;
	char ch;
	while (!feof(_fp))
	{
		ch = fgetc(_fp);
		if (ch != '\n' && ch != -1)
		{
			if (isspace(ch) && ch != ' ')
				continue;
			else
				*(beg++) = ch;
		}
		else
		{
			*beg = 0;
			return;
		}
	}
	_buf = filter(_buf);
	return;
}

long double check_num(char* _s, int* _len)//判断数字的合法性
{
	long double num = 0, w = 1, p = 0;
	int stat = 0;
	*_len = 0;
	while (stat != -1)
	{
		stat = am.trans_table[stat][am.attr[*_s]];
		if (!stat)
			break;
		++(*_len);
		if (stat == 2)
			num = num * 10 + *_s - '0';
		else if (stat == 4)
			num = num * 8 + *_s - '0';
		else if (stat == 5)
			num = num * 16 + isdigit(*_s) ? *_s - '0' : tolower(*_s) - 'a' + 10;
		else if (stat == 3)
			num += (*_s - '0') * (w /= 10);
		else if (stat == 6)
			p = p * 10 + *_s - '0';
		++_s;
	}
	if (stat == -1)
		return -1;
	else
		return num * pow(10, p);
}
int check_num_type(char* _s, int _len)//硬编码一波问题不大
{
	int i = 0;
	int is_dec = 0, is_u = 0, is_l = 0, is_f = 0;
	while (i++ < _len)
	{
		if (*_s == '.' || *_s == 'e')
			++is_dec;
		else if (*_s == 'u')
			++is_u;
		else if (*_s == 'l')
			++is_l;
		else if (*_s == 'f')
			++is_f;
	}
	if (is_dec)
		return is_f ? 13 : (is_l ? 12 : 11);
	else
	{
		if (is_u && is_l == 0)
			return 6;
		if (is_u && is_l == 1)
			return 8;
		if (is_u && is_l == 2)
			return 10;
		if (!is_u && is_l == 1)
			return 7;
		if (!is_u && is_l == 2)
			return 9;
		return 5;
	}
}

int check_id(char* _s, int* _len)//判断保留字
{
	char* p = _s;
	*_len = 0;
	while (isalnum(*p) || *p == '_')
		++p, ++(*_len);
	string* tmp = string_init_by_len(_s, *_len);
	hs_tb_n* node = hs_tb_find_node(res_hs_tb, tmp);
	string_destroy(tmp);
	if (node)
		return *(int*)node->val;
	return -1;
}

int check_ope(char* _s, int* _len)//判断操作符
{
	char* p = _s;
	*_len = 0;
	string* tmp = string_init("");
	hs_tb_n* node = NULL;
	int preval = 0;
	do
	{
		string_append_by_len(tmp, p, 1);
		node = hs_tb_find_node(ope_hs_tb, tmp);
		if (!node)
			break;
		++p, ++(*_len);
		preval = *(int*)node->val;
	} while (*p && !(isalnum(*p) || *p == '_' || *p == ' '));
	string_destroy(tmp);
	return *_len ? preval : -1;
}

void check_string(char* _s, int* _len)
{
	char* p = _s;
	*_len = 0;
	while (*p && *p != '\"')
		++p, ++(*_len);
	if (!(*p))
		*_len = -1;
}

void check_char(char* _s, int* _len)
{
	char* p = _s;
	*_len = 0;
	while (*p && *p != '\'')
		++p, ++(*_len);
	if (!(*p))
		*_len = -1;
}
void source_to_token(vector* _tk_list, char* _s)
{
	token tk;
	long double sav_con;
	int tmp;
	data_arg str_arg = { sizeof(string),string_assign,string_destroy };
	data_arg long_arg = { sizeof(long double),NULL,NULL };
	string* stmp = string_init("");
	while (*_s)
	{
		tmp = 0;
		if (*_s == ' ')
		{
			++_s;
			continue;
		}
		else if (*_s == '#')
			return;
		else if (isdigit(*_s) || (*_s == ' ' && isdigit(*(_s + 1))))
		{
			sav_con = check_num(_s, &tmp);
			if (sav_con == -1)
				catch_exce(7);
			set_token(&tk, numb + check_num_type(_s, tmp), &sav_con, long_arg);//数字
			_s += tmp;
		}
		else if (isalpha(*_s))
		{
			int type_id = check_id(_s, &tmp);
			if (type_id == -1)
			{
				string_reset_by_len(stmp, _s, tmp);
				set_token(&tk, name, stmp, str_arg);//标识符
			}
			else
				set_token(&tk, type_id, NULL, { sizeof(void*),NULL,NULL });//保留字
			_s += tmp;
		}
		else
		{
			int ope_type = check_ope(_s, &tmp);
			if (ope_type == d_quote)
			{
				++_s;
				check_string(_s, &tmp);
				if (tmp == -1)
					catch_exce(9);
				string_reset_by_len(stmp, _s, tmp);
				set_token(&tk, str_n, stmp, str_arg);
				_s += tmp + 1;
			}
			else if (ope_type == s_quote)
			{
				++_s;
				check_char(_s, &tmp);
				if (tmp == -1)
					catch_exce(9);
				string_reset_by_len(stmp, _s, tmp);
				set_token(&tk, ch_n, stmp, str_arg);
				_s += tmp + 1;
			}
			else
			{
				set_token(&tk, ope_type, NULL, { sizeof(void*),NULL,NULL });
				if (ope_type == -1)
					catch_exce(8);
				_s += tmp;
			}
		}
		vec_push_back(_tk_list, &tk);
		token_clear(&tk);
	}
	string_destroy(stmp);
}

const int buf_size = 10005;
const char* num_type[10] =
{
	"int",		"unsigned int",
	"long",		"unsigned long",
	"long long","unsigned long long",
	"double",	"long double",
	"float"
};
void print_token(token* _tk)
{
	if (_tk->id >= 120)
		printf("null %d", _tk->id);
	else if (_tk->id >= 105)
		printf("%s, %Lf", num_type[_tk->id - 105], *((long double*)_tk->dat));
	else if (_tk->id == 101)
		printf("id, %s", ((string*)_tk->dat)->buf);
	else if (_tk->id == 102)
		printf("string, %s", ((string*)_tk->dat)->buf);
	else if (_tk->id == 103)
		printf("char, %s", ((string*)_tk->dat)->buf);
	else
	{
		printf("%d ", _tk->id);
		if (_tk->id < res_rng)
			printf("%s", res_lab[_tk->id]);
		else if (_tk->id < d_ope_rng)
			printf("%s", d_ope[_tk->id - res_size]);
		else
			printf("%s", b_ope[_tk->id - d_ope_rng]);
	}
}

void get_token_list(vector* _vec, FILE* fp)
{
	char* buf = (char*)malloc(buf_size);
	table_init();
	auto_init();
	int i = 0;
	while (!feof(fp))
	{
		get_a_line_and_filter(fp, buf);
		source_to_token(_vec, buf);
	}
	table_destroy();
	free(buf);
}