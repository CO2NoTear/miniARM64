#include "DFA.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "opt.h"

//全局公共子表达式消除
int global_comsub_opt(BB **BB_array, int BB_num, TAC * tac_first){
    int exp_num;
    R_node * res[BB_num];
    My_exp * exp_list[1000];
    exp_num = Available_expression(BB_array, BB_num, tac_first, res, exp_list);
    for(int i = 0; i < BB_num; i ++){
        for(TAC *cur = BB_array[i]->in; (cur != NULL) && (cur != BB_array[i]->out->next); cur = cur->next){
            //判断当前TAC是否为表达式型
            if(cur->op >= 1 && cur->op <= 11){
                //如果是，则找到当前表达式在exp_list中的下标j
                int j = 0;
                j = find_exp(cur, exp_num, exp_list);
                //如果当前BB的R_node的in_vector有此表达式，则说明该表达式可用
                if((j != -1) && (res[i]->in_vector[j] == 1)){
                    //检查cur的两个源操作数是否在之前被定义过
                    int redef_fl;
                    redef_fl = is_redef(BB_array[i]->in, cur);
                    if(redef_fl == 0){
                        bl_prebb(i, BB_array, cur);
                    }
                }
            }  
        }
    }
}

//遍历当前BB的所有前驱BB中的所有TAC，找是否有跟cur表达式一样的TAC
void bl_prebb(int bb_i, BB **BB_array, TAC *cur){
    //遍历当前BB的所有前驱BB
    for(A_node *tmp = BB_array[bb_i]->prev; tmp != NULL; tmp = tmp->next){
        //flag，指示当前前驱是否有TAC与cur表达式相同，0为没有，1为有
        int f = 0;
        //自底向上遍历前驱BB的每条表达式型TAC
        for(TAC *prev_cur = BB_array[tmp->n]->out; (prev_cur != NULL) && (prev_cur != BB_array[tmp->n]->in->prev); prev_cur = prev_cur->prev){
            if((prev_cur->op >=1) && (prev_cur->op <= 10)){
                int same_flag;
                same_flag = is_same_exp(cur, prev_cur);
                if(same_flag == 1){
                    strcpy(cur->next->b->name, prev_cur->a->name);
                    cur->prev->prev->next = cur->next;
                    cur->next->prev = cur->prev->prev;
                    f = 1;
                }
            }
        }
        //若遍历完前驱BB的所有TAC后f仍为0则遍历该前驱的前驱
        if(f == 0){
            bl_prebb(tmp->n, BB_array, cur);
        }
    }
}

//判断两条表达式型TAC的表达式是否相同.相同返回1；否则返回0
int is_same_exp(TAC *ta, TAC *tb){
    if(ta->b->type == 1 && ta->c->type == 1){
        int sb,sc,so;
        sb = same_var(ta->b, tb->b);
        sc = same_var(ta->c, tb->c);
        so = (ta->op == tb->op) ? 1 : 0;
        if(sb == 1 && sc == 1 && so == 1){
            return 1;
        }
        else{
            return 0;
        }
    }
    if(ta->b->type == 1 && ta->c->type == 4){
        int sb,sc,so;
        sb = same_var(ta->b, tb->b);
        sc = same_cons(ta->c, tb->c);
        so = (ta->op == tb->op) ? 1 : 0;
        if(sb == 1 && sc == 1 && so == 1){
            return 1;
        }
        else{
            return 0;
        }
    }
    if(ta->b->type == 4 && ta->c->type == 1){
        int sb,sc,so;
        sb = same_cons(ta->b, tb->b);
        sc = same_var(ta->c, tb->c);
        so = (ta->op == tb->op) ? 1 : 0;
        if(sb == 1 && sc == 1 && so == 1){
            return 1;
        }
        else{
            return 0;
        }
    }
}

//如果源操作数被重定义了，返回1；没有则返回0
int is_redef(TAC *sch, TAC *curr){
    for(TAC * between = sch->next; (between != NULL) && (between != curr); between = between->next){
        if(between->op == 12){
            if(same_var(between->a, curr->b) || same_var(between->a, curr->c)){
                return 1;
            }
        }
    }
    return 0;
}