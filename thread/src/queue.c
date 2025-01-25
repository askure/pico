#include "../include/kernerl.h"
void add_queue(TCB ** queue, TCB *tcb)
{
    if(*queue == NULL){ 
        *queue = tcb;
        tcb->pre = tcb;
    }
    else{
        TCB *end;
        end = (*queue)->pre;
        end->next = tcb;
        tcb->pre = end;
        (*queue)->pre = tcb;
    }
}

void remove_queue(TCB ** queue, TCB *tcb){
    if(*queue == NULL){
        return;  
    }

    if(*queue == tcb){
        TCB *top = *queue;
        *queue = top->next;
        if(*queue != NULL){
            (*queue)->pre = top->pre;
        }
    }
    else{
        TCB *pre = tcb->pre;
        TCB *next = tcb->next;
        pre->next = next;
        if(next != NULL){
            next->pre = pre;
        }
        else{
            (*queue)->pre = pre;
        }
    }
}