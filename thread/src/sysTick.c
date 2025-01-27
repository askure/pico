#include "../include/kernerl.h"
TCB *wait_queue = NULL;
static int count;
void isr_systick(void){
    if(count >=100){
        TCB *tcb;
        for(tcb = wait_queue; tcb != NULL; tcb = tcb->next){
            remove_queue(&wait_queue,tcb);
            add_queue(&ready_queue,tcb);
        }
        remove_queue(&ready_queue,cur_task);
        add_queue(&wait_queue,cur_task);
        count = 0;
    }
    count++;
    schedule();
    return;
}

void print_sp(void *i){
    printf("%p\n",i);
}