#include "../include/kernerl.h"
TCB *wait_queue = NULL;
static int count;
static int n = 0;
int script_dispatch =0;
int pending_dispatch;
void change_tsk(){
    int i;
    if(cur_task != NULL){
        remove_queue(&ready_queue[cur_task->priority],cur_task);
        add_queue(&ready_queue[cur_task->priority],cur_task);
    }
    for(i=0;i<READY_NUM;i++) if(ready_queue[i] != NULL) break;
    cur_task  = ready_queue[i];
}
void schedule(){
    TCB * tcb;
    for(TCB * tcb = wait_queue; tcb != NULL; tcb = tcb->next){
        if(tcb->wait_time >= 10){
            tcb->wait_time-=10;
        }
        else{
            remove_queue(&wait_queue,tcb);
            add_queue(&ready_queue[tcb->priority],tcb);
            tcb = wait_queue;
        }
    }
    // if(pending_dispatch){
    //     printf("pending_dispatch\n");
    //     pending_dispatch = 0;
    //     change_tsk();
    // }
    change_tsk();   
    // if(count >=100){
    //     TCB *tcb;
    //     for(tcb = wait_queue; tcb != NULL; tcb = tcb->next){
    //         remove_queue(&wait_queue,tcb);
    //         add_queue(&ready_queue[tcb->priority],tcb);
    //     }
    //     remove_queue(&ready_queue[cur_task->priority],cur_task);
    //     add_queue(&wait_queue,cur_task);
    //     count = 0;
    // }
    if(count >100){
        dispatch();
        printf("end distpatch\n");
        count = 0;
    }
    count++;
    //schedule();
    return ;
}



void print(void* i){
    printf("print_lr:%p\n",i);
}
volatile void b(){
    printf("a");
}
