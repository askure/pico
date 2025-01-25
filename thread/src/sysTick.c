#include "../include/kernerl.h"
TCB *wait_queue;
static int count;
void isr_systick(void){
    if(count == 20){
        TCB * tcb = wait_queue;
        if(wait_queue != NULL){
            add_queue(&ready_queue,tcb);
            remove_queue(&wait_queue,tcb);
        }
        remove_queue(&ready_queue,cur_task);
        add_queue(&wait_queue,cur_task);
        count = 0;
    }
    schedule();
    count++;
    return;
}