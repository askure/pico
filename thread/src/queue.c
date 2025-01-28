#include "../include/kernerl.h"
void add_queue(TCB ** queue, TCB *tcb)
{   
    if(tcb == NULL){
        printf("tcb is NULL\n");
        return;
    }
    if(*queue == NULL){ 
        *queue = tcb;
        (*queue)->pre = tcb;
    }
    else{
        TCB *end;
        end = (*queue)->pre;
        end->next = tcb;
        tcb->pre = end;
        (*queue)->pre = tcb;
    }
    tcb->next = NULL;
    //printf("[%p] add\n",tcb);
}

int remove_queue(TCB ** queue, TCB *tcb){
    int err = -1;
    if(*queue == NULL){
        printf("queue is NULL\n");
        return err ;  
    }
    
    for(TCB *tmp = *queue; tmp != NULL; tmp = tmp->next){
        if(tmp == tcb){
            err = 0;
            break;
        }
    } 
    if(err){
        printf("tcb is not in queue\n");
        return err;
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
    // printf("[%p] remove\n",tcb);
    // printf("[%p] head\n",*queue);
}