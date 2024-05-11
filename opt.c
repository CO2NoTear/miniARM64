#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "opt.h"

#define MAX_ROUND 10

struct Optimizers{
    // all optimizers
    int (* simple_opt)(TAC * tac_first);
    int (* const_opt)(BB **BB_array, int BB_num, TAC * tac_first);
    int (* copy_propagaton_opt)(BB **BB_array, int BB_num, TAC * tac_first);
    int (* deadcode_opt)(BB **BB_array, int BB_num, TAC * tac_first);
    int (* deadvar_opt)(BB **BB_array, int BB_num, TAC * tac_first);
    int (* local_comsub_opt)(BB **BB_array, int BB_num);
    int (* global_comsub_opt)(BB **BB_array, int BB_num, TAC * tac_first);
};    

// pointer array, representing the CFG
BB *BB_array[MAX_BB];

int cmp (const TAC **a, const TAC **b){
    return ((*a)->id >= (*b)->id);
}

TAC * find_tac(int type, SYM *a, SYM *b, SYM *c){
    for(TAC *cur = tac_first; cur != NULL; cur = cur->next){
        if(cur->op == type && cur->a == a && cur->b == b && cur->c == c){
            // printf("success!\n");
            return cur;
        }
    }
    printf("Fail to find a correct TAC!\n");
    exit(-1);
}

// args: id is the target TAC id; num is the BB nums
// 有个奇怪的情况是一个BB会有两个Lable
int find_BB(int id, int num){
    for(int i = 0; i <= num; i++){
        if(id >= BB_array[i]->in->id && id <= BB_array[i]->out->id) return i;
    }
    printf("Fail to find the target BB!\n");
    exit(-1);
}


int build_CFG(TAC *tac_first){
    // Set the id for all TAC, start from 0
    int tmp = 0;
    for(TAC *cur = tac_first; cur !=NULL; cur=cur->next){
        cur->id = tmp ++;
    }

    // get all BBs for the first, assuming you are familiar with this algorithm
    TAC *headers[MAX_tac];
    int foo[MAX_tac] = {0}; // use for deduplication
    int cur_headers = 0;

    // the entry TAC
    headers[cur_headers ++] = tac_first;
    foo[0] = 1;

    for(TAC *cur = tac_first->next; cur !=NULL; cur=cur->next){
        if(cur->op == TAC_GOTO){
            // need to find the GOTO target
            TAC *tmp_tac = find_tac(TAC_LABEL, cur->a, NULL, NULL);
             
            // add into headers array
            if(foo[tmp_tac->id] != 1){
                foo[tmp_tac->id] = 1;
                headers[cur_headers ++] = tmp_tac;
            }
        }
        else if(cur->op == TAC_IFZ){
            //SYM a is the LABLE, so we do the same
            TAC *tmp_tac = find_tac(TAC_LABEL, cur->a, NULL, NULL);
            if(foo[tmp_tac->id] != 1){
                foo[tmp_tac->id] = 1;
                headers[cur_headers ++] = tmp_tac;
            }

            // also , the next TAC is the header
            if(foo[cur->next->id] != 1){
                foo[cur->next->id] = 1;
                headers[cur_headers ++] = cur->next;
            }
        }
    }

    // find all the headers, now we should sort them and build the BB
    qsort(headers, cur_headers, sizeof(BB *), cmp);

    for(int i = 0; i < cur_headers - 1 ; i++){
        BB* tmp = (BB *)malloc(sizeof(BB));
        tmp->in = headers[i];
        tmp->out = headers[i + 1]->prev;
        tmp->id = i;
        tmp->suc = NULL;
        tmp->live = 1;
        BB_array[i] = tmp;
    }

    
    // last BB need specially processing. Note that cur_headers equals to the number of headers and the number of BBs
    BB* tmp2 = (BB *)malloc(sizeof(BB));
    BB_array[cur_headers - 1] = tmp2;
    BB_array[cur_headers - 1]->in = headers[cur_headers - 1];
    BB_array[cur_headers - 1]->out = tac_last;
    BB_array[cur_headers - 1]->id = cur_headers - 1;
    BB_array[cur_headers - 1]->suc = NULL;
    BB_array[cur_headers - 1]->live = 1;

    // build the CFG, note that the last BB have no successors
    for(int i = 0; i < cur_headers - 1; i++){
        if(BB_array[i]->out->op == TAC_GOTO){
            TAC *tmp = find_tac(TAC_LABEL, BB_array[i]->out->a, NULL, NULL);
            int bb_tmp = find_BB(tmp->id, cur_headers - 1);

            // Find the GOTO target BB, now we can create a adjust list node
            A_node *tmp1 = (A_node *)malloc(sizeof(A_node));
            tmp1->n = bb_tmp;
            tmp1->next = NULL;

            BB_array[i]->suc = tmp1;
        }
        else if(BB_array[i]->out->op == TAC_IFZ){

            // we have 2 TAC to process
            TAC *tac_tar = find_tac(TAC_LABEL, BB_array[i]->out->a, NULL, NULL);
            TAC *tac_next = BB_array[i]->out->next;
            int bb_tmp = find_BB(tac_tar->id, cur_headers - 1);

            // suc1 is the IFZ target
            A_node *tmp_tar = (A_node *)malloc(sizeof(A_node));
            tmp_tar->n = bb_tmp;
            tmp_tar->next = (A_node *)malloc(sizeof(A_node));
            // suc2 is the next BB
            tmp_tar->next->n = i + 1;
            tmp_tar->next->next = NULL;

            BB_array[i]->suc = tmp_tar;
        }
        else{
            // BB caused by other jmp
            A_node *tmp1 = (A_node *)malloc(sizeof(A_node));
            tmp1->n = i + 1;
            tmp1->next = NULL;
            BB_array[i]->suc = tmp1;
        }
    }

    for(int i = 0; i < cur_headers ; i ++){
        A_node *tmp = BB_array[i]->suc;
        if(tmp != NULL){
            // there is a BB have a prev headers[i]
            A_node * new_prev = (A_node *)malloc(sizeof(A_node));
            new_prev->n = i;
            new_prev->next = NULL;

            int target_BB = tmp->n;
            // we should find the tail of the prev list.
            if(BB_array[target_BB]->prev != NULL){
                A_node *t = BB_array[target_BB]->prev;
                while(t->next != NULL) t = t->next;
                // t is the nearest A_node
                t->next = new_prev;
            }else BB_array[target_BB]->prev = new_prev;

            // may have 2 successors
            if(tmp->next != NULL){
                tmp = tmp->next;
                
                new_prev = (A_node *)malloc(sizeof(A_node));
                new_prev->n = i;
                new_prev->next = NULL; 
                // repeat!
                target_BB = tmp->n;
                if(BB_array[target_BB]->prev != NULL){
                    A_node *t = BB_array[target_BB]->prev;
                    while(t->next != NULL) t = t->next;
                    // t is the nearest A_node
                    t->next = new_prev;
                }else BB_array[target_BB]->prev = new_prev;
            }
        }
    }

    // return the number of BBs, for further DFA
    return cur_headers;
}   

int simple_opt(TAC * tac_first){
    int opt_count = 0;
    // iterate throurh all the TAC and delete x = x
    for(TAC *tmp = tac_first ; tmp != NULL ; ){
        if(tmp -> op == TAC_COPY){
            if(tmp->a->type == SYM_VAR && tmp->b->type == SYM_VAR){
                if(strcmp(tmp->a->name, tmp->b->name) == 0){
                    // we not free the memory
                    tmp->prev->next = tmp->next;
                    tmp->next->prev = tmp->prev;
                    opt_count ++;
                }
            }
        }
        
        tmp = tmp->next;
    }

    return opt_count;
}
void dfs(BB **BB_array, int BB, int *reached_BB){
    reached_BB[BB] = 1;
    A_node* suc;
    for(suc = BB_array[BB]->suc; suc!=NULL;suc=suc->next){
        if(!reached_BB[suc->n]){
            dfs(BB_array,suc->n,reached_BB);
        }
    }
}

void delete_from_suc(BB **BB_array, int suc, int delete_num){
    A_node * tmp;
    if(BB_array[suc]->prev->n == delete_num && BB_array[suc]->prev->next == NULL){
        //只剩一个prev,这个prev就是要删除的节点
        BB_array[suc]->prev = NULL;
    }
    // else if(BB_array[suc]->prev->n != delete_num && BB_array[suc]->prev->next == NULL){

    // }
    else if(BB_array[suc]->prev->n == delete_num && BB_array[suc]->prev->next != NULL){
        //剩两个prev,这个prev在第一个
        BB_array[suc]->prev = BB_array[suc]->prev->next;
    }
    else if(BB_array[suc]->prev->next->n == delete_num){
        //剩两个prev，这个prev在第二个
        BB_array[suc]->prev->next = NULL;
    }
}

void delete_from_prev(BB **BB_array, int prev, int delete_num){
    A_node * tmp;
    if(BB_array[prev]->suc == NULL){//前一个block已经被删了

    }
    else if(BB_array[prev]->suc->n == delete_num && BB_array[prev]->suc->next == NULL){
        //只剩一个suc,这个suc就是要删除的节点
        BB_array[prev]->suc = NULL;
    }
    else if(BB_array[prev]->suc->n != delete_num && BB_array[prev]->suc->next == NULL){
    }
    else if(BB_array[prev]->suc->n == delete_num && BB_array[prev]->suc->next != NULL){
        //剩两个suc,这个suc在第一个
        BB_array[prev]->suc = BB_array[prev]->suc->next;
    }
    else if(BB_array[prev]->suc->next->n == delete_num){
        //剩两个suc，这个suc在第二个
        BB_array[prev]->suc->next = NULL;
    }
}

int kill_BB(BB **BB_array, int BB_num, TAC * tac_first ,int delete_num){
    if(BB_array[delete_num]->live == 1){
        //change live sign
        BB_array[delete_num]->live = 0;
        //delete TAC
        if(delete_num == 0){
            tac_first = BB_array[delete_num]->out->next;
        }
        else if(delete_num == delete_num-1){
            BB_array[delete_num]->in->prev->next = NULL;
        }
        else{
            BB_array[delete_num]->in->prev->next = BB_array[delete_num]->out->next;
            BB_array[delete_num]->out->next->prev = BB_array[delete_num]->in->prev;
        }
        //delete its information from its suc and prev
        A_node* tmp;
        if(BB_array[delete_num]->suc!=NULL){
            for(tmp = BB_array[delete_num]->suc; tmp!=NULL; tmp=tmp->next){
                delete_from_suc(BB_array,tmp->n,delete_num);
            }
            BB_array[delete_num]->suc=NULL;
        }
        if(BB_array[delete_num]->prev!=NULL){
            for(tmp = BB_array[delete_num]->prev; tmp!=NULL; tmp=tmp->next){
                delete_from_prev(BB_array,tmp->n,delete_num);
            }
            BB_array[delete_num]->prev=NULL;
        }
        return 1;
    }
    else{
        return 0;
    }
}

// ldf写的惊为天人的11层嵌套
void dead_brunch(BB **BB_array, int BB_num){
    for(int i=0;i<BB_num;i++){
        if(BB_array[i]->live == 1){
            if(BB_array[i]->out->op==14){
                TAC* cur = BB_array[i]->out;
                if(cur->b->type==SYM_INT){
                    if(BB_array[i]->suc!=NULL && BB_array[i]->suc->next!=NULL){ //有两个后继的情况下才能裁剪
                        int label_num;
                        A_node* suc ;
                        for(suc= BB_array[i]->suc; suc!=NULL; suc=suc->next){
                            if(BB_array[suc->n]->in->op==17 && strcmp(BB_array[suc->n]->in->a->name,cur->a->name)==0){
                                label_num = suc->n;
                            }
                        }
                        if(cur->b->value==0){
                            //kill掉除了label_num的另一个
                            if(BB_array[i]->suc->n==label_num) BB_array[i]->suc->next=NULL;
                            else BB_array[i]->suc = BB_array[i]->suc->next;
                        }
                        else{
                            //kill掉label_num
                            if(BB_array[i]->suc->n==label_num) BB_array[i]->suc = BB_array[i]->suc->next;
                            else BB_array[i]->suc->next=NULL;
                        }
                    }
                }
                
            }
        }
    }
}

int deadvar_opt(BB **BB_array, int BB_num, TAC * tac_first){
    int opt_count = 0;
    R_node *res[BB_num];
    SYM *def_vars[MAX_var];
    int res_def_index;
    Live_Var(BB_array, BB_num, tac_first,res,def_vars,&res_def_index);
    TAC * cur;
    //根据live var analysis分析
    for(cur=tac_first;cur!=NULL;cur=cur->next){
        if(cur->op >= 1 && cur->op <= 12){
            for(int i =0;i<res_def_index;i++){
                if(strcmp(def_vars[i]->name,cur->a->name)==0 && cur->OUT_vector[i]!=1){//it's a dead assign
                    cur->prev->next = cur->next;
                    cur->next->prev = cur->prev;
                    for(int j=0;j<BB_num;j++){
                        if(cur == BB_array[j]->in) BB_array[j]->in = cur->next;
                        else if (cur == BB_array[j]->out) BB_array[j]->out = cur->prev;
                    }
                    opt_count++;
                }
            }
        }
        else if(cur->op == TAC_VAR){//TAC_VAR要为0的时候才能删除
            for(int i =0;i<res_def_index;i++){
                if(strcmp(def_vars[i]->name,cur->a->name)==0 && cur->OUT_vector[i]==0){//it's a dead assign
                    cur->prev->next = cur->next;
                    cur->next->prev = cur->prev;
                    for(int j=0;j<BB_num;j++){
                        if(cur == BB_array[j]->in) BB_array[j]->in = cur->next;
                        else if (cur == BB_array[j]->out) BB_array[j]->out = cur->prev;
                    }
                    opt_count++;
                }
            }
        }
    }
    return opt_count;
}


int deadcode_opt(BB **BB_array, int BB_num, TAC * tac_first){
    //int reached_BB[BB_num] = {0};
    int opt_count = 0;
    int * reached_BB = (int*)malloc(BB_num * sizeof(int));
    for(int i=0;i<BB_num;i++){
        reached_BB[i] = 0;
    }
    //detect dead branch
    dead_brunch(BB_array,BB_num);
    dfs(BB_array,0,reached_BB);
    // for(int i=0;i<BB_num;i++){
    //     printf("%d ",reached_BB[i]);
    // }
    // printf("\n");
    for (int i =0; i<BB_num;i++){
        if(reached_BB[i] == 0){
            opt_count+=kill_BB(BB_array,BB_num,tac_first,i);
        }
    }
    return opt_count;
}

int const_opt(BB **BB_array, int BB_num, TAC * tac_first){
    
    R_node *res[BB_num];
    SYM *vars[100];
    int vars_index = 0;

    // Constant Propagation Dataflow Analysis
    Constant_Propagation(BB_array, BB_num, tac_first, res ,vars, &vars_index);

    int opt_count = 0;
    // printf("before \n");
    // tac_dump();
    for(TAC * cur = tac_first; cur != NULL; cur = cur->next){
        if(cur->op >= 1 && cur->op <= 12){
            if(cur->op == TAC_COPY && cur->b->type == SYM_INT) continue;

            SYM *tmp = cur->a;
            // tac_print(cur);
            // printf("\n");
            // for(int i = 0; i < vars_index ; i++){
            //     printf("%s ", vars[i]->name);
            // }
            // printf("\n");
            
            int opt_index = find_var(tmp, vars, vars_index);

            if(cur->Val_Status[opt_index] == DEF){
                if(cur->b->type == SYM_INT){
                    // 已经优化过了，等号右侧为常数
                    continue;
                }
                // Find a TAC that can be optimized.
                cur->op = TAC_COPY;
                SYM *new_b = (SYM *)malloc(sizeof(SYM));
                new_b -> type = SYM_INT;
                new_b -> value = cur->OUT_vector[opt_index];
                cur->b = new_b;

                opt_count ++;
            }
        }
        else if(cur->op == TAC_IFZ){

            if(cur->b->type == SYM_INT) continue;
            
            SYM *tmp = cur->b;
            // tac_dump();
            int opt_index = find_var(tmp, vars, vars_index);

            if(cur->Val_Status[opt_index] == DEF){
                // Find a IFZ that can be optimized.
                SYM *new_b = (SYM *)malloc(sizeof(SYM));
                new_b -> type = SYM_INT;
                new_b -> value = cur->OUT_vector[opt_index];
                cur->b = new_b;

                opt_count ++;
            }

        }
    }
    return opt_count;
}

int find_tac_in_deftac(TAC *cur, TAC ** def_tac, int index){
    for(int i = 0; i < index ; i++){
        if(cur == def_tac[i]) return i;
    }
    printf("can not find a correct TAC! \n");
    exit(-1);
}

int copy_opt(BB **BB_array, int BB_num, TAC * tac_first){
    // 全局复制传播优化，这里需要把Reaching Definition的meet改为and
    R_node *res[BB_num];
    TAC *def_tac[100];
    int def_index = 0;

    int opt_cnt = 0;

    // Reaching Definition Dataflow Analysis
    Reaching_definition(BB_array, BB_num, tac_first, res, def_tac, &def_index, 0);

    
    for(TAC *cur = tac_first; cur != NULL; cur=cur->next){
        // find x = y
        if(cur->op == TAC_COPY && cur->b->type == SYM_VAR){
            int opt_index = find_tac_in_deftac(cur, def_tac, def_index);

            SYM *tmp = cur->a;
            // find all TAC that use tmp 
            for(TAC * tac_tmp = cur; tac_tmp != NULL; tac_tmp = tac_tmp->next){

                if(tac_tmp->op >= 1 && tac_tmp->op <= 12){

                    if(tac_tmp->OUT_vector[opt_index] == 1){
                        // check the Reaching Definition Results
                        if(tac_tmp->b == tmp) {
                            tac_tmp->b = cur->b;
                            opt_cnt ++;
                        }
                        if(tac_tmp->c == tmp) {
                            tac_tmp->c = cur->b;
                            opt_cnt ++;
                        }
                        
                    }
                }
            }
        }
    }

    return opt_cnt;
}


// //公共子表达式消除
// int comsub_opt(BB **BB_array, int BB_num, TAC * tac_first)
// {   
//     int opt_count = 0;
//     int exp_num;
//     R_node * res[BB_num];
//     My_exp * exp_list[1000];
//     exp_num = Available_expression(BB_array, BB_num, tac_first, res, exp_list);
//     // printf("表达式个数是:%d\n",exp_num);
//     //遍历CFG中的每个BB
//     for(int i = 0;i < BB_num;i ++){
//         //遍历每个BB中的每一条TAC
//         for(TAC * cur = BB_array[i]->in; (cur != NULL) && (cur != BB_array[i]->out->next); cur = cur->next){
//             //判断当前TAC是否为表达式型
//             if(cur->op >= 1 && cur->op <= 11){
//                 //如果是，则找到当前表达式在exp_list中的下标j
//                 int j = 0;
//                 j = find_exp(cur, exp_num, exp_list);
//                 //如果当前BB的R_node的in_vector有此表达式，则说明该表达式可用
//                 if((j != -1) && (res[i]->in_vector[j] == 1)){
//                     //检查在此BB中，cur之前是否有操作数被重定义过
//                     for(TAC * search_tac = BB_array[i]->in; (search_tac != NULL) && (search_tac != cur); search_tac = search_tac->next){
//                         if(search_tac->op == 12){
//                             //在cur之前cur的操作数没被重定义过
//                             if((!same_var(search_tac->a, cur->b)) && (!same_var(search_tac->a, cur->c))){
//                                 //回溯到最近的前驱BB，替换其前驱BB中的TAC
//                                 for(A_node *tmp = BB_array[i]->prev; tmp != NULL; tmp = tmp->next){
//                                     //遍历前驱BB中的每一条TAC
//                                     for(TAC * prev_cur = BB_array[tmp->n]->in; (prev_cur != NULL) && (prev_cur != BB_array[tmp->n]->out->next); prev_cur = prev_cur->next){
//                                         //检查当前TAC是否是表达式型,若是则比较prev_cur和cur的b c op
//                                         if(prev_cur->op >=1 && prev_cur->op <=11){
//                                             int sb,sc,so;
//                                             if(cur->b->type == 4 && cur->c->type == 1){
//                                                 sb = same_cons(cur->b, prev_cur->b);
//                                                 sc = same_var(cur->c, prev_cur->c);
//                                             }
//                                             if(cur->b->type == 1 && cur->c->type == 4){
//                                                 sb = same_var(cur->b, prev_cur->b);
//                                                 sc = same_cons(cur->c, prev_cur->c);
//                                             }
//                                             if(cur->b->type == 1 && cur->c->type == 1){
//                                                 sb = same_var(cur->b, prev_cur->b);
//                                                 sc = same_var(cur->c, prev_cur->c);
//                                             }
//                                             if(prev_cur->op == cur->op){
//                                                 so = 1;
//                                             }
//                                             else{
//                                                 so = 0;
//                                             }
//                                             //printf("sb等于%d,sc等于%d,so等于%d\n",sb,sc,so);
//                                             if(sb && sc && so){
//                                                 if(cur->next->op == 12){
//                                                     //不知道只改name行不行
//                                                     strcpy(cur->next->b->name, prev_cur->a->name);
//                                                     cur->prev->prev->next = cur->next;
//                                                     cur->next->prev = cur->prev->prev;
//                                                     opt_count += 1;
//                                                 }
//                                             }
//                                         }
//                                     }
//                                 }
//                             }
//                             // if((search_tac->a != cur->b) && (search_tac->a != cur->c)){
//                             //     //回溯到最近的前驱BB，替换其前驱BB中的TAC
//                             //     for(A_node *tmp = BB_array[i]->prev; tmp != NULL; tmp = tmp->next){
//                             //         //遍历前驱BB中的每一条TAC
//                             //         for(TAC * prev_cur = BB_array[tmp->n]->in; (prev_cur != NULL) && (prev_cur != BB_array[tmp->n]->out->next); prev_cur = prev_cur->next){
//                             //             //检查当前TAC是否包含此表达式
//                             //             if(prev_cur->b == cur->b && prev_cur->op == cur->op && prev_cur->c == cur->c){
//                             //             //向TAC链表中插入新节点，如 z = b op c 改成 bc = b op c;z = bc同时修改外循环中正遍历的TAC，a = b op c改成a = bc
//                             //             //新建符号bc
//                             //             // SYM * new_a = (SYM *)malloc(sizeof(SYM));
//                             //             // char * name = (char *)malloc(sizeof(char));
//                             //             // strcpy(name, prev_cur->b->name);
//                             //             // strcat(name, prev_cur->c->name);
//                             //             // new_a->name = name;
//                             //             // new_a->type = SYM_INT;
//                             //             // prev_cur->a = new_a;
//                             //             // //创建新TAC，用于存z = bc
//                             //             // TAC * tmp = (TAC *)malloc(sizeof(TAC));
//                             //             // tmp->a = prev_cur->a;
//                             //             // tmp->op = TAC_COPY;
//                             //             // tmp->b = new_a;
//                             //             // //在z = x op y后插入新TAC
//                             //             // tmp->next = prev_cur->next;
//                             //             // prev_cur->next = tmp;
//                             //             // //将当前TAC改为a = xopy
//                             //             // cur->b = new_a;
//                             //             // cur->op = TAC_COPY;

//                             //                 if(cur->next->op == 12){
//                             //                     cur->prev->next = cur->next;
//                             //                     cur->next->prev = cur->prev;
//                             //                     cur->next->b == prev_cur->a;
//                             //                     opt_count += 1;
//                             //                 }
//                             //             }
//                             //         }
//                             //     }
//                             // }
//                         }
//                     }
//                 }
//             }
//         }
//     }
//     return opt_count;
// }

//全局公共子表达式消除
int global_comsub_opt(BB **BB_array, int BB_num, TAC * tac_first){
    int exp_num;
    int opt_cnt;
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
                        opt_cnt += 1;
                    }
                }
            }  
        }
    }
    return opt_cnt;
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

//局部公共子表达式消除
int part_comsub_opt(BB **BB_array, int BB_num)
{
    int opt_count = 0;
    //遍历每个BB
    for(int i = 0;i < BB_num;i ++){
        //遍历BB中每一条表达式型TAC（tn = x op y)
        for(TAC * cur = BB_array[i]->in; (cur != NULL) && (cur != BB_array[i]->out->next); cur = cur->next){
            if(cur->op >= 1 && cur->op <= 10){
                //遍历cur的前驱（从cur自底向上遍历）
                for(TAC * search_tac = cur->prev; (search_tac != NULL) && (search_tac != BB_array[i]->in->prev); search_tac = search_tac->prev){
                    if(search_tac->op >= 1 && search_tac->op <= 10){
                        int same_flag, redef_flag;
                        same_flag = is_same_exp(cur, search_tac);
                        if(same_flag == 1){
                            redef_flag = is_redef(search_tac, cur);
                            if(redef_flag == 0){
                                strcpy(cur->next->b->name, search_tac->a->name);
                                cur->prev->prev->next = cur->next;
                                cur->next->prev = cur->prev->prev;
                                opt_count += 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return opt_count;
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

static const struct Optimizers do_opt = {
    .simple_opt = simple_opt,
    .const_opt = const_opt,
    .copy_propagaton_opt = copy_opt,
    .deadcode_opt = deadcode_opt,
    .deadvar_opt = deadvar_opt,
    .local_comsub_opt = part_comsub_opt,
    .global_comsub_opt = global_comsub_opt,
};

int tac_optimizer(){

    int BB_num = build_CFG(tac_first);
    // int opt_cnt;
    // opt_cnt = global_comsub_opt(BB_array,BB_num,tac_first);
    // printf("优化的条数是%d\n",opt_cnt);
    // printf("基本块数量是：%d\n",BB_num);
    // int k = part_comsub_opt(BB_array,BB_num);
    // printf("优化的条数为%d\n",k);

    // 不申请内存了，直接拉满1000，def_tac_index也传进去，函数返回后这个变量存def_tac数组的最大下标。
    // int def_tac_index = 0;
    // TAC * def_tac[1000];

    // Reaching_definition(BB_array, BB_num, tac_first, res, def_tac, &def_tac_index);

    // dbg
    // for(int i = 0; i < BB_num ; i ++){
    //     printf("RD rresult for BB : %d\n", res[i]->n);
    //     printf("IN_vector: \n");
    //     for(int j = 0; j < res[i]->count ; j++){
    //         printf("%d ", res[i]->in_vector[j]);
    //     }
    //     printf("\n");
    //     printf("OUT_vector: \n");
    //     for(int j = 0; j < res[i]->count ; j++){
    //         printf("%d ", res[i]->out_vector[j]);
    //     }
    //     printf("\n");
    // }





    // Available_expression(BB_array, BB_num, tac_first, res);

    // printf(" -------------------- 下面是常量传播分析-js --------------------\n");


    // for(int i = 0; i < BB_num ; i ++){
    //     printf("RD rresult for BB : %d\n", res[i]->n);
    //     printf("IN_vector: \n");
    //     for(int j = 0; j < res[i]->count ; j++){
    //         printf("%d ", res[i]->in_vector[j]);
    //     }
    //     printf("\n");
    //             printf("in status:\n");
    //     for(int j = 0; j < res[i]->count ; j++){
    //         printf("%d ", res[i]->statue_in[j]);
    //     }   
    //     printf("\n");
    //     printf("OUT_vector: \n");
    //     for(int j = 0; j < res[i]->count ; j++){
    //         printf("%d ", res[i]->out_vector[j]);
    //     }
    //     printf("\n");
    //     printf("status:\n");
    //     for(int j = 0; j < res[i]->count ; j++){
    //         printf("%d ", res[i]->constant_status[j]);
    //     }        
    //     printf("\n");
    // }


    // All things done, now we begin to do optimize!
    int opt_cnt;
    int round = 0;
    do{
        opt_cnt = 0;
        // 简单优化
        // X = X
        opt_cnt += do_opt.simple_opt(tac_first);
        // 常量传播+常量折叠优化
        opt_cnt += do_opt.const_opt(BB_array, BB_num, tac_first);
        // 全局和局部复制传播优化
        opt_cnt += do_opt.copy_propagaton_opt(BB_array, BB_num, tac_first);
        // 死代码消除
        opt_cnt += do_opt.deadcode_opt(BB_array,BB_num,tac_first);
        // deadassign消除
        opt_cnt += do_opt.deadvar_opt(BB_array,BB_num,tac_first);
        // 公共子表达式消除
        opt_cnt += do_opt.local_comsub_opt(BB_array, BB_num);

        opt_cnt += do_opt.global_comsub_opt(BB_array,BB_num,tac_first);

        round ++;
    }while(opt_cnt > 0 && round <= MAX_ROUND);    
    // lxy test
    // R_node *res[BB_num];
    // SYM *def_vars[MAX_var];
    // int res_def_index;
    // Live_Var(BB_array, BB_num, tac_first,res,def_vars,&res_def_index);
    // TAC *cur;
    // for(cur=tac_first;cur!=NULL;cur=cur->next){
    //     tac_print(cur);
    //     printf("\n");
    //     for(int i=0;i<res_def_index;i++){
    //         printf("%d ", cur->OUT_vector[i]);
    //     }
    //     printf("\n");
    // }
    // do_opt.deadvar_opt(BB_array,BB_num,tac_first);
}
