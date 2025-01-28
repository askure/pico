#include "../include/kernerl.h"
TCB *wait_queue = NULL;
static int count;
static int n = 0;
void schedule(void){
    //printf("test %d\n",count);
    // for(TCB * tmp = wait_queue; tmp != NULL; tmp = tmp->next){
    //     if(tmp->wait_time >= 10){
    //         tmp->wait_time-=10;
    //     }
    //     else{
    //         remove_queue(&wait_queue,tmp);
    //         add_queue(&ready_queue,tmp)
    //     }
    // }
    if(count > 100){
        int i;
        if(cur_task != NULL){
            remove_queue(&ready_queue[cur_task->priority],cur_task);
            add_queue(&ready_queue[cur_task->priority],cur_task);
        }
        for(i=0;i<2;i++) if(ready_queue[i] != NULL) break;
        TCB * tcb = ready_queue[i];
        cur_task = tcb;
        printf("tsk:%d\n",cur_task->tsk_id);
        // StackFrame* sfp = (StackFrame*)next_task->context;
        // printf("task->context->lr:%p\n",sfp->lr);
        count = 0;
    }
    count++;
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
    // count++;
    //schedule();
    return ;
}

void print(void* i){
    printf("print_lr:%p\n",i);
}
volatile void b(){
    printf("a");
}
void a(){
     //test_print();
     return;
}