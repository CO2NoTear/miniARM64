#ifndef DFA
#define DFA

#include "tac.h"
#include "obj.h"
#include "opt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

#define DEF 1
#define NAC 2
#define UNDEF 0

typedef struct worklist{
    int data[MAX_SIZE];
    int front;
    int rear;
}WL;

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

    // this is used ONLY for constant propagation!
    int * constant_status;
    int * status_in;

}R_node;

typedef struct my_expression
{
    SYM *b;
    SYM *c;
    int op;
}My_exp;


WL * init_WL(void);
int is_empty(WL *wl);
int is_full(WL *wl);
int get_BB_from_WL(WL *wl);
int insert_BB_to_WL(WL *wl, int BB);

#endif