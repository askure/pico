#include "../include/kernerl.h"
TCB *wait_queue = NULL;
static int count;
static int n = 0;
void test_print(int i){
    //printf("test %d\n",count);
    if(count > 100){
        if(n == 0) n = 1;
        else n = 0;
        cur_task = &tcb_tbl[n];
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
    printf("print_sp:%p\n",i);
}
volatile void b(){
    printf("a");
}
void a(){
     printf("isr_systick\n");
     //test_print();
     return;
}