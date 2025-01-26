#include "../include/kernerl.h"
TCB tcb_tbl[TCB_NUM];
int create_tsk(void * stackaddr, int32_t stacksize, void (*fp)(),void (*end)())
{   
    int intsts;
    DI(intsts);
    int i;
    for(i=0;i< TCB_NUM;i++) if(tcb_tbl[i].state == NONEXIT) break;
    if(i >= TCB_NUM){
        printf("task is full\n"); 
        i= -1;
    }
    else{
        tcb_tbl[i].context = make_context(stackaddr, stacksize, fp, end);
        tcb_tbl[i].stackaddr = stackaddr;
        tcb_tbl[i].stacksize = stacksize;
        tcb_tbl[i].fp = fp;
        tcb_tbl[i].end = end;
        tcb_tbl[i].state = DORAMENT;
        tcb_tbl[i].pre = NULL;
        tcb_tbl[i].next = NULL;

    }
    EI(intsts);
    return i;
}

void tsk_run(){
    int intsts;
    DI(intsts);
    for(int i=0 ; i< TCB_NUM; i++){
        if(tcb_tbl[i].state == DORAMENT){
            tcb_tbl[i].state = READY;
            add_queue(&ready_queue, &tcb_tbl[i]);
            printf("ready:%p , tcb:%p\n",ready_queue,&tcb_tbl[i]);
        }
    }
    schedule();
    EI(intsts);
}

void tsk_sleep(uint32_t ms){
    int intsts;
    DI(intsts);
    if(ms > 0 && cur_task != NULL){
        remove_queue(&ready_queue,cur_task);
        cur_task->wait_time = ms;
        add_queue(&wait_queue,cur_task);    
    }
    EI(intsts);
    schedule();
}