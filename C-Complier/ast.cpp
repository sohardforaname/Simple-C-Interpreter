#include "ast.h"
#include "source.h"
stack* ops, * ids;
void ast_stack_init()
{
	ops = stack_init(null_arg);
	ids = stack_init(null_arg);
}
void ast_stack_destroy()
{
	stack_destroy(ops);
	stack_destroy(ids);
}

void print_tree_1(tree_node* t)
{
    if (t->dat) {
        print_token((token*)t->dat);
        printf("\n");
    }
    if (t->ch[0])
        print_tree_1(t->ch[0]);
    if (t->ch[1])
        print_tree_1(t->ch[1]);
}

tree_node* build_expr(const vector* _vec, size_t* _size);
tree_node* build_block(const vector* _vec, size_t* _size);
tree_node* build_val_decl(const vector* _vec, size_t* _size);
tree_node* build_func_decl(const vector* _vec, size_t* _size);
tree_node* build_if_while(const vector* _vec, size_t* _size, int tag);

void debug(token* tk) 
{
    printf("error in syntax: \n");
    print_token(tk);
    printf("\n");
    catch_exce(11);
}
tree_node* build_expr(const vector* _vec, size_t* _size) //进入函数的第一个token是id, 不跳过结束符
{
    tree_node* node = set_ast_node(expr_stmt, NULL, null_arg);
    tree_node** ptr = &(node->ch[0]);
    token* tk = vec_get_data(_vec, *_size, token);
    while (tk->id != 71
        && tk->id != 64 && tk->id != 66) {
        if ((tk->id >= 32 && tk->id < 63) || tk->id == 72) {
            if (!ops->size)
                stack_push_no_copy(ops, set_ast_node_by_token(tk));
            else {
                token* tp = (token*)((tree_node*)stack_top(ops))->dat;
                while (ops->size && get_prev(tk->id) > get_prev(tp->id)) {
                    token* tp = (token*)((tree_node*)stack_top(ops))->dat;
                    if (ids->size <= 1)
                        catch_exce(5);
                    tree_node* top_n = (tree_node*)stack_top_and_pop_no_delete(ops);
                    top_n->ch[0] = (tree_node*)stack_top_and_pop_no_delete(ids);
                    top_n->ch[0]->ch[1] = (tree_node*)stack_top_and_pop_no_delete(ids);
                    stack_push_no_copy(ids, top_n);
                    
                }
                stack_push_no_copy(ops, set_ast_node_by_token(tk));
            }
            //++(*_size);
        } else if (tk->id >= 100) {
            tree_node* nd = set_ast_node_by_token(tk);
            tree_node** ptr1 = &(nd->ch[0]);
            if (tk->id == 101) {
                if (*_size + 1 != _vec->size) {
                    token* nxttk = vec_get_data(_vec, *(_size) + 1, token);
                    if (nxttk->id == 65 || nxttk->id == 63) { // 左圆括号和左方括号
                        *_size += 2;
                        *ptr1 = build_expr(_vec, _size);
                        ptr1 = &((*ptr1)->ch[1]);
                        if (*_size >= _vec->size
                            || nxttk->id != vec_get_data(_vec, *_size, token)->id - 1)
                            catch_exce(11);
                    }
                }
            } else if (tk->id >= 125) {               
                debug(tk);
            }
            //++(*_size);
            stack_push_no_copy(ids, nd);
        } else if (tk->id == 63) {
            ++(*_size); // 跳过左括号
            *ptr = build_expr(_vec, _size);
            ptr = &((*ptr)->ch[1]);
            if (*_size >= _vec->size
                || tk->id != vec_get_data(_vec, *_size, token)->id - 1)
                catch_exce(11);
            //++(*_size);
        } else {
            debug(tk);
        }
        ++(*_size);
        tk = vec_get_data(_vec, *_size, token);
    }
    while (ops->size) {
        if (ids->size <= 1)
            catch_exce(5);
        tree_node* tp = (tree_node*)stack_top_and_pop_no_delete(ops);
        tp->ch[0] = (tree_node*)stack_top_and_pop_no_delete(ids);
        tp->ch[0]->ch[1] = (tree_node*)stack_top_and_pop_no_delete(ids);
        stack_push_no_copy(ids, tp);
    }
    *ptr = (tree_node*)stack_top_and_pop_no_delete(ids);
    ptr = &((*ptr)->ch[1]);
    if (ops->size || ids->size)
        catch_exce(5);
    // *ptr = set_ast_node_by_token(tk);
    return node;
}
tree_node* build_val_decl(const vector* _vec, size_t* _size) //进入函数的第一个token是type, 跳过分号
{
    tree_node* node = set_ast_node(decl_stmt, NULL, null_arg);
    tree_node* ptr = node;
    ptr->ch[0] = set_ast_node_by_token(vec_get_data(_vec, (*_size)++, token));
    ptr = ptr->ch[0];
    if (*_size >= _vec->size)
        catch_exce(4);
    token* tk = vec_get_data(_vec, *_size, token);
    if (tk->id != 101)
        catch_exce(11);
    ptr->ch[1] = build_expr(_vec, _size);
    tk = vec_get_data(_vec, *_size, token);
    if (tk->id != 71)
        catch_exce(11);
    ++(*_size); // 跳过分号
    return node;
}
tree_node* build_func_decl(const vector* _vec, size_t* _size) //进入函数的第一个token是type, 跳过分号
{
    tree_node* node = set_ast_node(decl_stmt, NULL, null_arg);
    tree_node* ptr = node;
    ptr->ch[0] = set_ast_node_by_token(vec_get_data(_vec, (*_size)++, token));
    ptr = ptr->ch[0];
    if (*_size >= _vec->size || vec_get_data(_vec, *_size, token)->id != 101)
        catch_exce(11);
    ptr->ch[1] = set_ast_node_by_token(vec_get_data(_vec, (*_size)++, token));
    token* tk = vec_get_data(_vec, (*_size)++, token);
    if (*_size >= _vec->size || tk->id != 63)
        catch_exce(11);
    while (1) {
        tk = vec_get_data(_vec, *_size, token);
        if (tk->id == 64)
            break;
        ++(*_size);
        if (*_size >= _vec->size || tk->id > 32)
            catch_exce(11);
        ptr->ch[1] = set_ast_node_by_token(tk);
        ptr = ptr->ch[1];
        tk = vec_get_data(_vec, (*_size)++, token);
        if (*_size >= _vec->size || tk->id !=101)
            catch_exce(11);
        ptr->ch[1] = set_ast_node_by_token(tk);
        ptr = ptr->ch[1];
        tk = vec_get_data(_vec, *_size, token);
        if (tk->id == 72)
            ++(*_size);
        else
            debug(tk);
    }
    ++(*_size);
    return node;
}
tree_node* build_block(const vector* _vec, size_t* _size) 
{
    tree_node* node = set_ast_node(bloc_stmt, NULL, null_arg);
    tree_node** ptr = &(node->ch[0]);
    token* tk = vec_get_data(_vec, (*_size)++, token);
    if (tk->id == 67) 
    {
        tk = vec_get_data(_vec, *_size, token);
        while (tk->id != 68) {
            if (tk->id >= 100) {
                *ptr = build_expr(_vec, _size);
                ptr = &((*ptr)->ch[1]);
                if (*_size >= _vec->size
                    || vec_get_data(_vec, *_size, token)->id != 71)
                    catch_exce(11);   
                 ++(*_size);
            } else if (tk->id == 15 || tk->id == 31) {
                *ptr = build_if_while(_vec, _size, tk->id == 31);
                ptr = &((*ptr)->ch[1]);
                /*if (*_size >= _vec->size
                    || vec_get_data(_vec, *_size, token)->id != 67)
                    catch_exce(11);*/
                //++(*_size);
            } else if (tk->id < 32) {
                *ptr = build_val_decl(_vec, _size);
                ptr = &((*ptr)->ch[1]);
            } else {
                debug(tk);
            }
            if (*_size >= _vec->size)
                catch_exce(11);
            tk = vec_get_data(_vec, *_size, token);
        }
        ++(*_size);
        return node;
    }
    catch_exce(11);
    return 0;
}
tree_node* build_if_while(const vector* _vec, size_t* _size, int tag)
{
    tree_node* node = set_ast_node(tag ? while_stmt : if_stmt, NULL, null_arg);
    tree_node* ptr = node;
    ++(*_size);
    if (*_size >= _vec->size || vec_get_data(_vec, *_size, token)->id != 63)
        catch_exce(11);
    ++(*_size);
    ptr->ch[1] = build_expr(_vec, _size);
    ptr = ptr->ch[1];
    if (vec_get_data(_vec, *_size, token)->id != 64)
        catch_exce(11);
    ++(*_size); // 跳过右括号

    // parse block;
    ptr->ch[1] = build_block(_vec, _size);

    return node;
}
bitree* build_ast(const vector* _vec)
{
    if (!_vec)
        catch_exce(6);
    size_t size = 0;
    bitree* btr = bitree_init(token_arg);
    btr->rt->ch[0] = set_ast_node(sen_beg, NULL, null_arg);
    tree_node* ptr = btr->rt->ch[0];
    while (size < _vec->size) {
        if (size + 2 >= _vec->size) {
            catch_exce(4);
        }
        token* tk = vec_get_data(_vec, size + 2, token);
        if (tk->id == 71) {
            ptr->ch[1] = build_val_decl(_vec, &size);
            ptr = ptr->ch[1];
        } else if (tk->id == 63) {
            ptr->ch[1] = build_func_decl(_vec, &size);
            ptr = ptr->ch[1];
            if (vec_get_data(_vec, size, token)->id == 71)
                continue;
            ptr->ch[1] = build_block(_vec, &size);
            ptr = ptr->ch[1];
        }
    }
    return btr;
}