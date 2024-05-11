#include "DFA.h"

TAC * def_tac[1000];


// output of DFA: each node representing the IN and OUT state of a BB
// one node <--> one BB
typedef struct res_node
{
    // n index the BB
    int n;
    // number of elems in vector
    int count;
    // IN and OUT vector for DFA
    __int8_t * in_vector;
    __int8_t * out_vector;

}R_node;


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

int find_def(TAC *cur_tac, int def_index){
    for(int i = 0; i < def_index; i ++){
        if(def_tac[i] == cur_tac) return i;
    }
    return -1;
}

// Bit Union, res stored in arr1
void Union_operation(__int8_t * arr1, __int8_t * arr2, int n){
    for(int i = 0; i<n;i++){
        arr1[i] = arr1[i] | arr2[i];
    }
}

// Bit Intersection, res stored in arr1
void Inter_operation(__int8_t * arr1, __int8_t * arr2, int n){
    for(int i = 0; i<n;i++){
        arr1[i] = arr1[i] & arr2[i];
    }
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
int RD_for_one_BB(BB **BB_array, int BB, R_node * BB_res, R_node **res, int def_index, SYM **def_vars){
    __int8_t * tmp_union;
    
    if(BB != 0){
        //Union all the prev OUT to get IN of this BB
        tmp_union = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
        
        // the first prev of this BB is first_prev
        int first_prev = BB_array[BB]->prev->n;
        
        memcpy(tmp_union, res[first_prev]->out_vector, sizeof(BB_res->count * sizeof(__int8_t)));

        // iteration all the prevs, tmp point to A_node representing a prev
        for(A_node *tmp = BB_array[BB]->prev; tmp != NULL; tmp = tmp->next){
            if(tmp->next != NULL){
                // Union the tmp_unio and the second prev's OUT
                __int8_t * need_to_union = res[tmp->next->n]->out_vector;
                Union_operation(tmp_union, need_to_union, BB_res->count);
            }
        }

        // Now tmp_union stores the res of all prev BB's OUT Union
        memcpy(BB_res->in_vector, tmp_union, BB_res->count);
    }
    else{
        // BB = 0, which means we do not need to Union
        tmp_union = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
        memcpy(tmp_union, BB_res->in_vector, sizeof(BB_res->count * sizeof(__int8_t)));
    }    

    // For compare
    __int8_t * old_OUT = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
    memcpy(old_OUT, BB_res->out_vector, sizeof(BB_res->count * sizeof(__int8_t)));

    TAC * cur;  

    // change OUT(tmp_union) for each TAC
    for(cur = BB_array[BB]->in; (cur != NULL) && (cur->prev != BB_array[BB]->out) ; cur = cur->next){  

        // go throuth def_tac to know this TAC's def_index. This is a dumb algorithm but we don not consider the workload :)
        int tmp_index = find_def(cur, def_index);
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
    }

    memcpy(BB_res->out_vector, tmp_union, sizeof(BB_res->count * sizeof(__int8_t)));

    for(int i = 0; i < BB_res->count ; i++){
        if(old_OUT[i] != BB_res->out_vector[i]) return 1;
    }

    return 0;
}

/*
    Input: CFG and TAC list
    Output: bit vectors for each BB
*/
void Reaching_definition(BB **BB_array, int BB_num, TAC *tac_first){
    // TODO: find how many Definitions in TAC
    int def_index = 0;
    SYM *def_vars[MAX_var];

    // result
    R_node *res[BB_num];

    for(TAC *cur = tac_first ; cur != NULL; cur = cur->next){
        if(cur->op >= 1 && cur->op <= 12){
            // Then is a Definition, we should know which var this def binding to. Share index def_index
            def_vars[def_index] = cur->a;
            def_tac[def_index ++] = cur;
        }
    }                   

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
        insert_BB_to_WL(wl, i);
    }

    // repeatly conduct Reaching definition
    while(!is_empty(wl)){
        int tmp_BB = get_BB_from_WL(wl);
        // printf("BB- %d\n", tmp_BB);
        // Do RD for this BB
        int flag = RD_for_one_BB(BB_array, tmp_BB, res[tmp_BB], res, def_index, def_vars);

        if(flag){
            // is the OUT changed, add all the successor BB to WL.
            for(A_node *tmp = BB_array[tmp_BB]->suc; tmp != NULL ; tmp = tmp->next){
                insert_BB_to_WL(wl, tmp->n);
            }
        }
    }

    // dbg
    for(int i = 0; i < BB_num ; i ++){
        printf("RD rresult for BB : %d\n", res[i]->n);
        printf("IN_vector: \n");
        for(int j = 0; j < res[i]->count ; j++){
            printf("%d ", res[i]->in_vector[j]);
        }
        printf("\n");
        printf("OUT_vector: \n");
        for(int j = 0; j < res[i]->count ; j++){
            printf("%d ", res[i]->out_vector[j]);
        }
        printf("\n");
    }
} 

int AE_for_one_BB(BB ** BB_array, int BB, R_node * BB_res, R_node ** res, int def_index, SYM ** def_vars) {
    __int8_t * tmp_inter;

    if (BB != 0) {
        // Intersect all the prev OUT to get IN of this BB
        tmp_inter = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));

        // the first prev of this BB is first_prev
        int first_prev = BB_array[BB]->prev->n;

        memcpy(tmp_inter, res[first_prev]->out_vector, sizeof(BB_res->count * sizeof(__int8_t)));

        // iteration all the prevs, tmp point to A_node representing a prev
        for (A_node * tmp = BB_array[BB]->prev; tmp != NULL; tmp = tmp->next) {
            if (tmp->next != NULL) {
                // Intersect the tmp_union and the second prev's OUT
                __int8_t * need_to_intersect = res[tmp->next->n]->out_vector;
                Inter_operation(tmp_inter, need_to_intersect, BB_res->count);
            }
        }
    }
    else {
        // BB = 0, do not need to Intersect
        tmp_inter = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
        memcpy(tmp_inter, BB_res->in_vector, sizeof(BB_res->count * sizeof(__int8_t)));
    }

    // For compare
    __int8_t * old_OUT = (__int8_t *)malloc(BB_res->count * sizeof(__int8_t));
    memcpy(old_OUT, BB_res->out_vector, sizeof(BB_res->count * sizeof(__int8_t)));

    TAC * cur;

    // change OUT(tmp_inter) for each TAC 遍历基本块BB中的每一条TAC
    for (cur = BB_array[BB]->in; (cur != NULL) && (cur->prev != BB_array[BB]->out); cur = cur->next) {

        // go through def_tac to know this TAC's def_index.
        int tmp_index = find_def(cur, def_index);
        if (tmp_index == -1) continue;

        SYM * kill_var = def_vars[tmp_index];

        // KILL expressions that are defined in this TAC
        for (int i = 0; i < BB_res->count; i++) {
            if (tmp_inter[i] == 1 && def_vars[i] == kill_var) {
                // KILL the expression
                tmp_inter[i] = 0;
            }
        }

        // GEN new expressions in this TAC
        tmp_inter[tmp_index] = 1;
    }

    memcpy(BB_res->out_vector, tmp_inter, sizeof(BB_res->count * sizeof(__int8_t)));

    for (int i = 0; i < BB_res->count; i++) {
        if (old_OUT[i] != BB_res->out_vector[i]) return 1;
    }

    return 0;
}

/*
    Input: CFG and TAC list
    Output: bit vectors for each BB
*/
void Available_Expressions(BB ** BB_array, int BB_num, TAC * tac_first) {
    // TODO: find how many Definitions in TAC
    int def_index = 0;
    SYM * def_vars[MAX_var];

    // result
    R_node * res[BB_num];

    for (TAC * cur = tac_first; cur != NULL; cur = cur->next) {
        if (cur->op >= 1 && cur->op <= 12) {
            // Then is a Definition, we should know which var this def binding to. Share index def_index
            def_vars[def_index] = cur->a;
            def_tac[def_index++] = cur;
        }
    }

    // TODO: initialize data structure
    for (int i = 0; i < BB_num; i++) {
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

    // create a new WL for this available expressions analysis, init it using all the BBs
    WL * wl = init_WL();
    for (int i = 0; i < BB_num; i++) {
        insert_BB_to_WL(wl, i);
    }

    // repeatedly conduct Available Expressions analysis
    while (!is_empty(wl)) {
        int tmp_BB = get_BB_from_WL(wl);
        // printf("BB- %d\n", tmp_BB);
        // Do AE for this BB
        int flag = AE_for_one_BB(BB_array, tmp_BB, res[tmp_BB], res, def_index, def_vars);

        if (flag) {
            // If the OUT set changed, add all the successor BBs to WL.
            for (A_node * tmp = BB_array[tmp_BB]->suc; tmp != NULL; tmp = tmp->next) {
                insert_BB_to_WL(wl, tmp->n);
            }
        }
    }

    // dbg
    for (int i = 0; i < BB_num; i++) {
        printf("AE result for BB : %d\n", res[i]->n);
        printf("IN_vector: \n");
        for (int j = 0; j < res[i]->count; j++) {
            printf("%d ", res[i]->in_vector[j]);
        }
        printf("\n");
        printf("OUT_vector: \n");
        for (int j = 0; j < res[i]->count; j++) {
            printf("%d ", res[i]->out_vector[j]);
        }
        printf("\n");
    }
}