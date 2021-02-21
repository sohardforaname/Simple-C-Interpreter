#include "def.h"
#define hs_tb hash_table
#define hs_tb_n hash_table_node
#define get_hash_val(_hst, _key) (_hst->hash.hash_func(_key) % _hst->capacity)
enum status { _EMPTY_, _EXIST_, _DELETE_ };
struct hash_table_node
{
	void* key;
	void* val;
	status st;
};
struct key_hash
{
	int (*is_equ)(const void* a, const void* b);
	unsigned long (*hash_func)(const void* _src);
};
struct hash_table
{
	hs_tb_n* buf;
	size_t size, capacity;
	data_arg key_arg, val_arg;
	key_hash hash;
};
hs_tb* hs_tb_init(data_arg _key_arg, data_arg val_arg, key_hash _key_hs);
hs_tb_n* const hs_tb_find_node(hs_tb* _hst, const void* _key);
size_t hs_tb_insert_node(hs_tb* _hst, const void* _key, const void* _val);
size_t hs_tb_set_node_delete(hs_tb* _hst, const void* _key);
void hs_tb_clear(hs_tb* _hst);
void* hs_tb_destroy(void* _hst);
void* hs_tb_assign(const void* _hst);