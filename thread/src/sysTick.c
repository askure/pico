#include "../include/kernerl.h"
TCB *wait_queue = NULL;
static int count;
void isr_systick(void){
    TCB *tcb;
    for(tcb = wait_queue; tcb != NULL; tcb = tcb->next){
        if(tcb->wait_time >= 10){
            tcb->wait_time -=10;
        }
        else{
            remove_queue(&wait_queue,tcb);
            add_queue(&ready_queue[tcb->pri],tcb);
            tcb = wait_queue;
        }   
    }
    if(count >=100){       
        remove_queue(&ready_queue[cur_task->pri],cur_task);
        add_queue(&ready_queue[cur_task->pri],cur_task);
        count = 0;
    }
    count++;
    schedule();
    return;
}

void print_sp(void *i){
    //printf("%p\n",i);
}