#include "../include/kernerl.h"
TCB *wait_queue = NULL;
static int count;
static int n = 0;
int test_print(int i){
    printf("test %d\n",i);
    if(count > 100){
        // if(n == 0) n = 1;
        // else n = 0;
        cur_task = tcb_tbl[0].context;
        printf("cur:%p\n",cur_task);
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
    return i+1;
}

void print(int i){
    printf("%x\n",i);
}
volatile void b(){
    printf("a");
}
void a(){
     printf("isr_systick\n");
     //test_print();
     return;
}