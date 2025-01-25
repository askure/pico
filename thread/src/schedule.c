#include "../include/kernerl.h"
int is_dispatch=0;
int count =0;
TCB *ready_queue;
TCB *cur_task;
TCB *next_task;
void schedule(){
    next_task = ready_queue;
    if(cur_task != next_task && !is_dispatch){
        dispatch();
    }
}