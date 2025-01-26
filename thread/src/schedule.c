#include "../include/kernerl.h"
int is_dispatch=0;
int count =0;
TCB *ready_queue = NULL;
TCB *cur_task = NULL;
TCB *next_task = NULL;
void schedule(){
    next_task = ready_queue;
    if(cur_task != next_task && !is_dispatch){
        dispatch();
    }
}