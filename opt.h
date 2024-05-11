#ifndef OPTH
#define OPTH

#include "tac.h"
#include "obj.h"
#include "DFA.h"

#define MAX_tac 1000
#define MAX_BB 100
#define MAX_var 100

typedef struct Adjacency_list_node{
    // n is the successor BB's id  
    int n;
    struct Adjacency_list_node* next;
}A_node;

typedef struct basic_block{
    // in and out TAC pointer of a BB
    TAC *in;
    TAC *out;
    // ID start from 0
    int id;
    // use Adjacency List to store the CFG, this is the pointer to successors
    A_node * suc;
    // prev BBs
    A_node * prev;

    //live sign
    int live;
}BB;


int tac_optimizer();


#endif