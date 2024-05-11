#include "DFA.h"


WL * init_WL(void){
    WL* tmp = (WL *)malloc(sizeof(WL));
    tmp->front = -1;
    tmp->rear = -1;
}

int is_empty(WL *wl){
    return (wl->front == wl->rear);
}

int is_full(WL *wl){
    return ((wl->rear + 1) % MAX_SIZE == wl->front);
}

int get_BB_from_WL(WL *wl){
    if(is_empty(wl)) return -1;
    wl->front = (wl->front + 1) % MAX_SIZE;
    return wl->data[wl->front];
}   

int insert_BB_to_WL(WL *wl, int BB){
    if(is_full(wl)) return -1;
    wl->rear = (wl->rear + 1) % MAX_SIZE;
    wl->data[wl->rear] = BB;
}

int find_def(TAC *cur_tac, int def_index, TAC ** def_tac){
    for(int i = 0; i < def_index; i ++){
        if(def_tac[i] == cur_tac) return i;
    }
    return -1;
}

// Bit Union, res stored in arr1
void Union_operation(__int8_t * arr1, __int8_t * arr2, int n, int control_bit){
    if(control_bit){
        for(int i = 0; i<n;i++){
            arr1[i] = arr1[i] | arr2[i];
        }
    }else{
        for(int i = 0; i<n;i++){
            arr1[i] = arr1[i] & arr2[i];
        }
    }

}

void Union_for_LV(__int8_t * arr1, __int8_t * arr2, int n, int control_bit){

    for(int i = 0; i<n;i++){
        // arr1[i] = arr1[i] | arr2[i];
        if(arr1[i]==1 || arr2[i]==1) arr1[i]=1;
        else if(arr1[i]==2 || arr2[i]==2) arr1[i]=2;
        else arr1[i]=0;}


}

// Realize the Meet operation and Transfer Function, return 1 if OUT has changed
// BB_res is the current BB, and res is the globle res for all BBs
/*
    input: 
    BB_array : global CFG
    BB : BB index which will be analysis this time
    BB_res : reaching definition analysis result of this BB
    res : reaching definition analysis result of all the BBs
    def_index : how many Definitions in this program
    def_vars : Var of each definition, using the same index with def_tac

    output:
    flag : whether OUT change of this BB. 1 => OUT changed
*/
int RD_for_one_BB(BB **BB_array, int BB, R_node * BB_res, R_node **res, int def_index, SYM **def_vars, TAC ** def_tac, int control_bit){

    __int8_t * tmp_union;
    
    if(BB != 0){
        //Union all the prev OUT to get IN of this BB
        tmp_union = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
        
        // the first prev of this BB is first_prev
        int first_prev = BB_array[BB]->prev->n;
        
        memcpy(tmp_union, res[first_prev]->out_vector, BB_res -> count);

        // iteration all the prevs, tmp point to A_node representing a prev
        for(A_node *tmp = BB_array[BB]->prev; tmp != NULL; tmp = tmp->next){
            if(tmp->next != NULL){
                // Union the tmp_unio and the second prev's OUT
                __int8_t * need_to_union = res[tmp->next->n]->out_vector;
                Union_operation(tmp_union, need_to_union, BB_res->count, control_bit);
            }
        }

        // Now tmp_union stores the res of all prev BB's OUT Union
        memcpy(BB_res->in_vector, tmp_union, BB_res->count);
    }
    else{
        // BB = 0, which means we do not need to Union
        tmp_union = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
        memcpy(tmp_union, BB_res->in_vector, BB_res -> count);
    }    

    // For compare
    __int8_t * old_OUT = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
    memcpy(old_OUT, BB_res->out_vector, BB_res -> count);

    TAC * cur;  

    // change OUT(tmp_union) for each TAC
    for(cur = BB_array[BB]->in; (cur != NULL) && (cur->prev != BB_array[BB]->out) ; cur = cur->next){  

        // go throuth def_tac to know this TAC's def_index. This is a dumb algorithm but we don not consider the workload :)
        int tmp_index = find_def(cur, def_index, def_tac);
        if(tmp_index == -1) continue;

        SYM * kill_var = def_vars[tmp_index];

        // KILL previous Definition on this VAR
        for(int i = 0 ; i < BB_res->count ; i ++ ){
            // find out all "1" in the current IN vector
            if(tmp_union[i] == 1 && def_vars[i] == kill_var){
                // KILL the previous definion on the var
                tmp_union[i] = 0;
            }
        }
        
        // set bit to 1 for this definition

        tmp_union[tmp_index] = 1;

        // we need the OUT of each TAC
        if(cur->OUT_vector != NULL) free(cur->OUT_vector);
        __int8_t * tmp_OUT = (__int8_t *)malloc(BB_res -> count);
        memcpy(tmp_OUT, tmp_union, BB_res -> count);
        cur->OUT_vector = tmp_OUT;
        
    }

    memcpy(BB_res->out_vector, tmp_union, BB_res -> count);
        
    for(int i = 0; i < BB_res->count ; i++){
        if(old_OUT[i] != BB_res->out_vector[i]) return 1;
    }

    return 0;
}

/*
    Input: CFG and TAC list
    Output: bit vectors for each BB

    control_bit: 原本的RD分析是一种may analysis，但是在复制传播场景下我认为应该用Must，所以多设置一个bit，当bit=1的时候
    进行may analysis，也就是传统的RD取并集。在复制传播的场景下bit = 0， 取交集。
*/
void Reaching_definition(BB **BB_array, int BB_num, TAC *tac_first, R_node ** res, TAC ** def_tac, int *def_tac_index, int control_bit){
    // TODO: find how many Definitions in TAC
    int def_index = 0;
    SYM *def_vars[MAX_var];

    // result
    // R_node *res[BB_num];

    for(TAC *cur = tac_first ; cur != NULL; cur = cur->next){
        if(cur->op >= 1 && cur->op <= 12){
            // Then is a Definition, we should know which var this def binding to. Share index def_index
            def_vars[def_index] = cur->a;
            def_tac[def_index ++] = cur;
        }
    }
    *def_tac_index = def_index;

    // TODO: initialize data structure
    for(int i = 0; i < BB_num; i++){
        res[i] = (R_node *)malloc(sizeof(R_node));
        res[i]->n = i;
        res[i]->count = def_index;
        res[i]->in_vector = (__int8_t *)malloc(def_index * sizeof(__int8_t));
        res[i]->out_vector = (__int8_t *)malloc(def_index * sizeof(__int8_t));

        memset(res[i]->in_vector, 0, def_index);
        memset(res[i]->out_vector, 0, def_index);
    }

    // TODO: realize the Transfer Function and Meet operation
    // TODO: carry out worklist algorithm for DFA

    // create a new WL for this reaching definition analysis, init it using all the BBs
    WL *wl = init_WL();
    for(int i = 0; i < BB_num; i ++){
        if(BB_array[i]->live == 1)  insert_BB_to_WL(wl, i);
    }

    // repeatly conduct Reaching definition
    while(!is_empty(wl)){
        int tmp_BB = get_BB_from_WL(wl);
        // printf("BB- %d\n", tmp_BB);
        // Do RD for this BB
        int flag = RD_for_one_BB(BB_array, tmp_BB, res[tmp_BB], res, def_index, def_vars, def_tac, control_bit);

        if(flag){
            // is the OUT changed, add all the successor BB to WL.
            for(A_node *tmp = BB_array[tmp_BB]->suc; tmp != NULL ; tmp = tmp->next){
                insert_BB_to_WL(wl, tmp->n);
            }
        }
    }
} 

int LV_for_one_BB(BB **BB_array, int BB, R_node * BB_res, R_node **res, int def_index, SYM **def_vars){
    __int8_t * tmp_union;
    if(BB_array[BB]->suc != NULL){
        //Union all the successor IN to get OUT of this BB
        tmp_union = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));

        // the first prev of this BB is first_prev
        int first_suc = BB_array[BB]->suc->n;
        memcpy(tmp_union, res[first_suc]->in_vector, BB_res->count);
    
        // iteration all the sucs, tmp point to A_node representing a suc
        for(A_node *tmp = BB_array[BB]->suc; tmp != NULL; tmp = tmp->next){
            if(tmp->next != NULL){
                // Union the tmp_unio and the second suc's IN
                __int8_t * need_to_union = res[tmp->next->n]->in_vector;
                Union_for_LV(tmp_union, need_to_union, BB_res->count, 1);
            }
        }

        // Now tmp_union stores the res of all prev BB's OUT Union
        memcpy(BB_res->out_vector, tmp_union, BB_res->count);
    }
    else{//当前BB为出口
        tmp_union = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
        memcpy(tmp_union, BB_res->out_vector, BB_res->count);
    }

    //For compare
    __int8_t * old_IN = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
    memcpy(old_IN, BB_res->in_vector, BB_res->count);
    TAC * cur;
    //change IN(tmp_union) for each TAC
    for(cur = BB_array[BB]->out; (cur != NULL) && (cur->next != BB_array[BB]->in) ; cur = cur->prev){  
        // tac_print(cur);
        // printf("%d",cur->op);
        // printf("\n");
        // we need the OUT of each TAC
        __int8_t * tmp_OUT = (__int8_t *)malloc(BB_res -> count);
        if(cur == BB_array[BB]->out){ //last TAC
            memcpy(tmp_OUT, BB_res->out_vector, BB_res -> count);
            cur->OUT_vector = tmp_OUT;
        }
        else{
            memcpy(tmp_OUT, tmp_union, BB_res -> count);
            cur->OUT_vector = tmp_OUT;
        }
        // tac_print(cur);
        // printf("\n");
        // for(int i=0;i<BB_res -> count;i++){
        //     printf("%d ", cur->OUT_vector[i]);
        // }
        // printf("\n");

        if(cur->op >= 1 && cur->op <= 12){
            // go throuth def_tac to know this TAC's def_index. This is a dumb algorithm but we don not consider the workload :)
            for(int i=0; i<def_index; i++){
                if(strcmp(def_vars[i]->name,cur->a->name)==0){
                    tmp_union[i] = 2;
                    break;
                }
            }
            if(cur->b != NULL && cur->b->name!=NULL){
                for(int i=0; i<def_index; i++){
                    if(strcmp(def_vars[i]->name,cur->b->name)==0){
                        tmp_union[i] = 1;
                        break;
                    }
                }
            }
            if(cur->c != NULL && cur->c->name!=NULL){
                for(int i=0; i<def_index; i++){
                    if(strcmp(def_vars[i]->name,cur->c->name)==0){
                        tmp_union[i] = 1;
                    }
                }
            }

        }
        else if (cur->op == 20 || cur->op == 22)//actual a & return a 
        {
            // tac_print(cur);
            for(int i=0; i<def_index; i++){
                if(strcmp(def_vars[i]->name,cur->a->name)==0){  
                    tmp_union[i] = 1;
                    break;
                }
            }
        }
        else if (cur->op == 14) //ifz b goto a
        {
            if(cur->b->type==SYM_VAR){
                for(int i=0; i<def_index; i++){
                    if(strcmp(def_vars[i]->name,cur->b->name)==0){
                        tmp_union[i] = 1;
                        break;
                    }
                }
            }
        }
        else
        {
            continue;
        }



    }
    
    memcpy(BB_res->in_vector, tmp_union, BB_res->count);

    for(int i = 0; i < BB_res->count ; i++){
        if(old_IN[i] != BB_res->in_vector[i]) return 1;
    }

    return 0;
}

void Live_Var(BB **BB_array, int BB_num,TAC * tac_first,R_node ** res,SYM **def_vars,int * res_def_index){
    // tac_dump();
    //TODO: find how many val in CFG
    // SYM *def_vars[MAX_var];
    int def_index = 0; 
    int flag = 0;
    //result 这里需要改一下_js
    // R_node *res[BB_num];
    for(TAC *cur = tac_first ; cur != NULL; cur = cur->next){
        if(cur->op == TAC_VAR){
            flag = 0;
            for(int i=0;i<def_index;i++){
                if(strcmp(cur->a->name, def_vars[i]->name) == 0){ //TODO:比较cur的a字符串和def_vars
                    flag = 1;
                    break;
                }
            }
            if(flag==0){
                def_vars[def_index] = cur->a;
                def_index++;
            }
        }
    }
    // printf("var num:%d\n",def_index); //test
    *res_def_index = def_index;

    // TODO: initialize data structure
    for(int i = 0; i < BB_num; i++){
        res[i] = (R_node *)malloc(sizeof(R_node));
        res[i]->n = i;
        res[i]->count = def_index;
        res[i]->in_vector = (__int8_t *)malloc(def_index * sizeof(__int8_t));
        res[i]->out_vector = (__int8_t *)malloc(def_index * sizeof(__int8_t));

        memset(res[i]->in_vector, 0, def_index);
        memset(res[i]->out_vector, 0, def_index);
    }

    // create a new WL for this reaching definition analysis, init it using all the BBs
    WL* wl = init_WL();
    // printf("%d\n",BB_array[BB_num-1]->live);
    for(int i = BB_num-1; i >=0; i --){
        if(BB_array[i]->live == 1){
            // printf("insert:%d\n",i);
            insert_BB_to_WL(wl, i);
        }
    }

    // repeatly conduct Reaching definition
    while(!is_empty(wl)){
        int tmp_BB = get_BB_from_WL(wl);
        // printf("BB- %d\n", tmp_BB);
        // Do LV for this BB
        
        int flag = LV_for_one_BB(BB_array, tmp_BB, res[tmp_BB], res, def_index, def_vars);

        if(flag){
            // is the IN changed, add all the predecessor BB to WL.
            for(A_node *tmp = BB_array[tmp_BB]->prev; tmp != NULL ; tmp = tmp->next){
                // printf("insert:%d\n",tmp->n);
                insert_BB_to_WL(wl, tmp->n);
            }
        }
    }
    //update each tac's in&out vector

        // dbg
    // for(int i = 0; i < BB_num ; i ++){
    //     printf("LV rresult for BB : %d\n", res[i]->n);
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
}

//ldf
//判断TAC中的表达式是否与exp_list[1000]中的表达式相同，若相同则返回下标
//exp_index:CFG中表达式的总数，在遍历所有TAC时得到该参数的值

int same_var(SYM *a, SYM *b){
    int result;
    //防止变量跟常数比时出现错误,如果b是常数，返回0
    if(b->type == 4){
        return 0;
    }
    result = strcmp(a->name, b->name);
    if(result == 0){
        return 1;
    }
    else{
        return 0;
    }
}

int same_cons(SYM *a, SYM *b){
    //防止常量跟变量比时出现错误，如果b是变量，返回0
    if(b->type == 1){
        return 0;
    }
    if(a->value == b->value){
        return 1;
    }
    else{
        return 0;
    }
}

int find_exp(TAC *cur_tac,int exp_index,My_exp **exp_list){
    int flag;
    flag = -1;

    //对exp_list中的表达式，匹配到就返回下标i，若遍历完后没匹配到，结束循环返回flag = -1
    for(int i = 0;i < exp_index;i ++){
    
        //cur_tac为“常数 op 变量”型  4：常数 1：变量
        if(cur_tac->b->type == 4 && cur_tac->c->type == 1){
            if(same_cons(cur_tac->b, exp_list[i]->b) && same_var(cur_tac->c, exp_list[i]->c) && cur_tac->op == exp_list[i]->op){
                return i;
            }
        }

        //cur_tac为“变量 op 常数”型 
        if(cur_tac->b->type == 1 && cur_tac->c->type == 4){
            if(same_var(cur_tac->b, exp_list[i]->b) && same_cons(cur_tac->c, exp_list[i]->c) && cur_tac->op == exp_list[i]->op){
                return i;
            }
        }

        //cur_tac为“变量 op 变量”型
        if(cur_tac->b->type == 1 && cur_tac->c->type == 1){
            if(same_var(cur_tac->b, exp_list[i]->b) && same_var(cur_tac->c, exp_list[i]->c) && cur_tac->op == exp_list[i]->op){
                return i;
            }
        }
    }
    return flag;
}

//判断exp_list中的变量是否被重新定义，若被重新定义则返回下标
int find_kill(TAC *cur_tac,int exp_index,My_exp **exp_list){
    for(int i = 0;i < exp_index;i ++){
        if(same_var(cur_tac->a, exp_list[i]->b) || same_var(cur_tac->a, exp_list[i]->c)){
            return i;
        }
        else{
            return -1;
        }
    }
}

// Bit Intersection, res stored in arr1
void Inter_operation(__int8_t * arr1, __int8_t * arr2, int n){
    for(int i = 0; i<n;i++){
        arr1[i] = arr1[i] & arr2[i];
    }
}

int AE_for_one_BB(BB **BB_array, int BB, R_node * BB_res, R_node **res, int exp_index, My_exp ** exp_list){

    __int8_t * tmp_inter;
    
    if(BB != 0){
        //Inter all the prev OUT to get IN of this BB
        tmp_inter = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
        
        // the first prev of this BB is first_prev
        int first_prev = BB_array[BB]->prev->n;
        
        memcpy(tmp_inter, res[first_prev]->out_vector, BB_res -> count);

        // iteration all the prevs, tmp point to A_node representing a prev
        for(A_node *tmp = BB_array[BB]->prev; tmp != NULL; tmp = tmp->next){
            if(tmp->next != NULL){
                // Union the tmp_unio and the second prev's OUT
                __int8_t * need_to_inter = res[tmp->next->n]->out_vector;
                Inter_operation(tmp_inter, need_to_inter, BB_res->count);
            }
        }

        // Now tmp_inter stores the res of all prev BB's OUT Union
        memcpy(BB_res->in_vector, tmp_inter, BB_res->count);
    }
    else{
        // BB = 0, which means we do not need to Union
        tmp_inter = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
        memcpy(tmp_inter, BB_res->in_vector, BB_res -> count);
    }    

    // For compare
    __int8_t * old_OUT = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
    memcpy(old_OUT, BB_res->out_vector, BB_res -> count);

    TAC * cur;  

    // change OUT(tmp_inter) for each TAC
    for(cur = BB_array[BB]->in; (cur != NULL) && (cur->prev != BB_array[BB]->out) ; cur = cur->next){ 
        int kill_i; 
        if(cur->op == 12){
            //tmp_inter[]中要被kill的下标
            kill_i = find_kill(cur, exp_index, exp_list);
            if(kill_i != -1){
                //遍历tmp_inter[]中的每个元素，若其下标等于kill_i且值为1，则置0
                for(int i = 0 ; i < BB_res->count ; i ++ ){
                    if(tmp_inter[i] == 1 && i == kill_i){
                        tmp_inter[i] = 0;
                    }
                }
            }
        }
        if(cur->op >= 1 && cur->op <= 11){
            int tmp_index = find_exp(cur, exp_index, exp_list);
            if(tmp_index != -1){
                // set bit to 1 for this expression
                tmp_inter[tmp_index] = 1;
            }
        }
    }

    memcpy(BB_res->out_vector, tmp_inter, BB_res -> count);
        
    for(int i = 0; i < BB_res->count ; i++){
        if(old_OUT[i] != BB_res->out_vector[i]) return 1;
    }

    return 0;
}

/*
    Input: CFG and TAC list
    Output: bit vectors for each BB
*/


int Available_expression(BB **BB_array, int BB_num, TAC *tac_first, R_node ** res, My_exp ** exp_list){
    int exp_index;
    exp_index = 0;
   //遍历CFG，将所有表达式指令中的表达式不重复地存到exp_list[]中
    for(TAC *cur = tac_first ; cur != NULL; cur = cur->next){
        if(cur->op >= 1 && cur->op <= 11){
            if(exp_index == 0){
                My_exp * tmp = (My_exp *)malloc(sizeof(My_exp));
                tmp->b = cur->b;
                tmp->c = cur->c;
                tmp->op = cur->op;
                exp_list[exp_index ++] = tmp;
                //printf("a当前tac的b的type为%d和c的type为%d\n",cur->b->type,cur->c->type);
                // printf("exp_list[0]是%s",exp_list[0]->b->value);
            }
            else{
                int f = 0;
                //printf("当前表达式个数为%d\n",exp_index);
                //printf("b当前tac的b的type为%d和c的type为%d\n",cur->b->type,cur->c->type);
                f = find_exp(cur, exp_index, exp_list);
                //printf("f的值为%d\n",f);
                if(f == -1){
                    // 没重复，加上新的一条
                    My_exp * tmp = (My_exp *)malloc(sizeof(My_exp));
                    tmp->b = cur->b;
                    tmp->c = cur->c;
                    tmp->op = cur->op;
                    exp_list[exp_index ++] = tmp;   
                    //printf("b当前tac的b的值为%d和c的名%s\n",cur->b->value,cur->c->name);                 
                }
            }
        }
    }
    
    // TODO: initialize data structure
    for(int i = 0; i < BB_num; i++){
        res[i] = (R_node *)malloc(sizeof(R_node));
        res[i]->n = i;
        res[i]->count = exp_index;
        res[i]->in_vector = (__int8_t *)malloc(exp_index * sizeof(__int8_t));
        res[i]->out_vector = (__int8_t *)malloc(exp_index * sizeof(__int8_t));

        memset(res[i]->in_vector, 0, exp_index);
        memset(res[i]->out_vector, 1, exp_index);
    }

    // TODO: realize the Transfer Function and Meet operation
    // TODO: carry out worklist algorithm for DFA

    // create a new WL for this reaching definition analysis, init it using all the BBs
    WL *wl = init_WL();
    for(int i = 0; i < BB_num; i ++){
        if(BB_array[i]->live == 1) insert_BB_to_WL(wl, i);
    }

    // repeatly conduct Reaching definition
    while(!is_empty(wl)){
        int tmp_BB = get_BB_from_WL(wl);
        // printf("BB- %d\n", tmp_BB);
        // Do RD for this BB
        int flag = AE_for_one_BB(BB_array, tmp_BB, res[tmp_BB], res, exp_index, exp_list);
        if(flag){
            // is the OUT changed, add all the successor BB to WL.
            for(A_node *tmp = BB_array[tmp_BB]->suc; tmp != NULL ; tmp = tmp->next){
                insert_BB_to_WL(wl, tmp->n);
            }
        }
    }

    //test
    //printf("exp num:%d\n",exp_index);
    // for(int i = 0; i < BB_num ; i ++){
    //     printf("AE result for BB : %d\n", res[i]->n);
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
    return exp_index;
}

void Constant_Union_operation(__int8_t * arr1, __int8_t * arr2, int n, int * status1, int * status2){
    // Union need val and status
    for(int i = 0; i < n ; i ++){
       switch(status1[i]){
        case UNDEF:
            if(status2[i] == DEF){
                // Change the function in algorithm
                status1[i] = UNDEF;
                arr1[i] = arr2[i];
            }
            else if(status2[i] == NAC){
                status1[i] = NAC;
            }
            break;
        case DEF:
            if(status2[i] == DEF){
                if(arr1[i] == arr2[i]);
                else status1[i] = NAC;
            }
            else if(status2[i] = NAC){
                status1[i] = NAC;
            }
            break;
        case NAC:
            break;
       }

    }

}

int find_var(SYM * sym, SYM ** def_vars, int index){
    // for(int i = 0; i< index;i++){
    //     printf("%x %s\n", def_vars[i], def_vars[i]->name);
    // }
    // printf("\n");
    // printf("%x %s\n", sym, sym->name);
    for(int i = 0; i< index; i++){
        if(strcmp(sym->name, def_vars[i]->name) == 0)  return i;
    }
    printf("can not find a def_var\n");
    // int *p;
    // p = 0x0;
    // *p = 1;
    return -1;
}

int do_simple_op(int op, int b, int c){
    switch(op){
        case TAC_ADD: return b + c;
        case TAC_SUB: return b - c;
        case TAC_MUL: return b * c;
        case TAC_DIV: return b / c;
        case TAC_EQ: return b == c;
        case TAC_NE: return b != c;
        case TAC_LT: return b < c;
        case TAC_GT: return b > c;
        case TAC_LE: return b <= c;
        case TAC_GE: return b >= c;
    }
}

int CP_for_one_BB(BB **BB_array, int BB, R_node * BB_res, R_node ** res, SYM **def_vars, int def_index){

    // Note that the Res of CP should be (x, val), but we just use out_vector: the i'th bit index the i'th varible in def_vars. 
    __int8_t * tmp_union;
    int * tmp_status;
    if(BB != 0){
        tmp_union = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
        tmp_status = (int *)malloc(BB_res ->count * sizeof(int));

        int first_prev = BB_array[BB]->prev->n;
        memcpy(tmp_union, res[first_prev]->out_vector, BB_res -> count);
        // need the status
        memcpy(tmp_status, res[first_prev]->constant_status, BB_res->count * sizeof(int));

        for(A_node *tmp = BB_array[BB]->prev; tmp != NULL; tmp = tmp->next){
            if(tmp->next != NULL){
                // Union the tmp_unio and the second prev's OUT. Constant propagation has complex union operation.
                __int8_t * need_to_union = res[tmp->next->n]->out_vector;
                int * status_for_second = res[tmp->next->n]->constant_status;

                Constant_Union_operation(tmp_union, need_to_union, BB_res->count, tmp_status, status_for_second);
            }
        }

        // Now tmp_union stores the res of all prev BB's OUT Union, so it is IN of this BB
        memcpy(BB_res->in_vector, tmp_union, BB_res->count);        
        memcpy(BB_res->status_in, tmp_status, BB_res->count* sizeof(int));
        memcpy(BB_res->constant_status, tmp_status, BB_res->count * sizeof(int));
    }
    else{
        // BB is the first BB
        tmp_union = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
        tmp_status = (int *)malloc(BB_res ->count * sizeof(int));
        memcpy(tmp_union, BB_res->in_vector, BB_res -> count);
        memcpy(tmp_status, BB_res->constant_status, BB_res -> count* sizeof(int));
    }

    //Status tells everything
    int * old_status = (int *)malloc(BB_res->count * sizeof(int));
    memcpy(old_status, BB_res->constant_status, BB_res -> count * sizeof(int));

    TAC * cur;  

    // change OUT(tmp_union) and status for each TAC
    for(cur = BB_array[BB]->in; (cur != NULL) && (cur->prev != BB_array[BB]->out) ; cur = cur->next){
    
        if(cur->op == 12){
            // index the status[] and OUT[]/IN[] 
            int a_index = find_var(cur->a, def_vars, def_index);
            if(cur->b->type == SYM_INT){
                tmp_status[a_index] = DEF;
                tmp_union[a_index] = cur->b->value;
            }
            else if(cur->b->type = SYM_VAR){
                
                int origin_index = find_var(cur->b, def_vars, def_index);
                if(origin_index == -1){
                    // 优化异常，删多了一些

                }
                tmp_status[a_index] = tmp_status[origin_index];
                tmp_union[a_index] = tmp_union[origin_index];                
            }
        }

        else if(cur->op >= 1 && cur->op <= 10){
            int a_index = find_var(cur->a, def_vars, def_index);
            // a= b op c
            int b_index, c_index;
            // -1 means var is a constant
            if(cur->b->type == SYM_INT) b_index = -1;
            else b_index = find_var(cur->b, def_vars, def_index);

            if(cur->c->type == SYM_INT) c_index = -1;
            else c_index = find_var(cur->c, def_vars, def_index);

            if(b_index == -1 || c_index == -1){
                // have a constant
                if(b_index == c_index == -1){
                    int a_val = do_simple_op(cur->op, cur->b->value, cur->c->value);
                    tmp_status[a_index] = DEF;
                    tmp_union[a_index] = a_val;
                }
                else if(b_index == -1){
                    // b is a constant and c is a var
                    tmp_status[a_index] = tmp_status[c_index];
                    if(tmp_status[c_index] == DEF){
                        tmp_union[a_index] = do_simple_op(cur->op, cur->b->value, tmp_union[c_index]);
                    }
                }
                else{
                    //b is a var
                    tmp_status[a_index] = tmp_status[b_index];
                    if(tmp_status[b_index] == DEF){
                        tmp_union[a_index] = do_simple_op(cur->op, tmp_union[b_index], cur->c->value);
                    }                    
                }
            }
            else{
                // now we shuold calculate b op c
                if(tmp_status[b_index] == UNDEF || tmp_status[c_index] == UNDEF){
                    tmp_status[a_index] == UNDEF;
                }
                else if(tmp_status[b_index] == NAC || tmp_status[c_index] == NAC)
                {
                    tmp_status[a_index] == NAC;
                }
                else{
                    int b_val = tmp_union[b_index];
                    int c_val = tmp_union[c_index];
                    tmp_union[a_index] = do_simple_op(cur->op, b_val, c_val);
                    tmp_status[a_index] = DEF;
                }
            }
        }
        else if(cur->op == TAC_NEG){
            int a_index = find_var(cur->a, def_vars, def_index);
            // a = -b
            int b_index;
            if(cur->b->type == SYM_INT) b_index = -1;
            else b_index = find_var(cur->b, def_vars, def_index);

            if(b_index == -1){
                tmp_union[a_index] = - (cur->b->value);
                tmp_status[a_index] = DEF;
            }
            else{
                tmp_status[a_index] = tmp_status[b_index];
                if(tmp_status[b_index] == DEF){
                    tmp_union[a_index] = - tmp_union[b_index];
                }
            }
            
        }

        // we need the IN and OUT of each TAC
        __int8_t * tmp_OUT = (__int8_t *)malloc(BB_res -> count);
        int * tmp_S = (int *)malloc(BB_res->count * sizeof(int));
        memcpy(tmp_OUT, tmp_union, BB_res -> count);
        memcpy(tmp_S, tmp_status, BB_res->count * sizeof(int));

        cur->OUT_vector = tmp_OUT;
        cur->Val_Status = tmp_S;
    }

    memcpy(BB_res->out_vector, tmp_union, BB_res -> count);
    memcpy(BB_res->constant_status, tmp_status, BB_res -> count * sizeof(int));

    for(int i = 0; i < BB_res->count ; i++){
        if(old_status[i] != BB_res->constant_status[i]) return 1;
    }
    return 0;
}


void Constant_Propagation(BB **BB_array, int BB_num, TAC *tac_first, R_node ** res, SYM **def_vars, int *var_index){
    int def_index = 0;
    int flag ;

    // all the vars
    for(TAC *cur = tac_first ; cur != NULL; cur = cur->next){
        if(cur->op == TAC_VAR){
            flag = 0;
            for(int i=0;i<def_index;i++){
                if(strcmp(cur->a->name, def_vars[i]->name) == 0){ //TODO:比较cur的a字符串和def_vars
                    flag = 1;
                    break;
                }
            }
            if(flag == 0){
                def_vars[def_index ++] = cur->a;
            }
        }
    }

    *var_index = def_index;
    // printf("vars : %d\n", def_index);

    //init res data structure. Note that Constant pro need EXTRA data structure!
    for(int i = 0; i < BB_num; i++){
        res[i] = (R_node *)malloc(sizeof(R_node));
        res[i]->n = i;
        res[i]->count = def_index;
        res[i]->in_vector = (__int8_t *)malloc(def_index * sizeof(__int8_t));
        res[i]->out_vector = (__int8_t *)malloc(def_index * sizeof(__int8_t));
        res[i]->constant_status = (int *)malloc(def_index * sizeof(int));
        res[i]->status_in = (int *)malloc(def_index * sizeof(int));

        memset(res[i]->in_vector, 0, def_index);
        memset(res[i]->out_vector, 0, def_index);
        memset(res[i]->constant_status, 0, def_index * sizeof(int));
        memset(res[i]->status_in, 0, def_index * sizeof(int));
    }

    WL *wl = init_WL();
    for(int i = 0; i < BB_num; i ++){
        if(BB_array[i]->live == 1) insert_BB_to_WL(wl, i);
    }

    while(!is_empty(wl)){
        int tmp_BB = get_BB_from_WL(wl);
        // printf("BB- %d\n", tmp_BB);
        // Do RD for this BB
        int flag = CP_for_one_BB(BB_array, tmp_BB, res[tmp_BB], res, def_vars, def_index);

        if(flag){
            // is the OUT changed, add all the successor BB to WL.
            for(A_node *tmp = BB_array[tmp_BB]->suc; tmp != NULL ; tmp = tmp->next){
                insert_BB_to_WL(wl, tmp->n);
            }
        }
    }
}