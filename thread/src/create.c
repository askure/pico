#include "../include/kernerl.h"
TCB tcb_tbl[TCB_NUM];
TCB empty_tcb;
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
        tcb_tbl[i].pri = 0;

    }
    EI(intsts);
    return i;
}
void empty(){
    while(1);
}
uint32_t empty_stack[1024/sizeof(uint32_t)];
void create_empty_tsk(){
    empty_tcb.context = make_context(empty_stack ,sizeof(empty_stack),empty,NULL);
    empty_tcb.state = READY;
    empty_tcb.pre = NULL;
    empty_tcb.next =  NULL;
    empty_tcb.pri = 1;
}

void tsk_run(){
    int intsts;
    DI(intsts);
    for(int i=0 ; i< TCB_NUM; i++){
        if(tcb_tbl[i].state == DORAMENT){
            tcb_tbl[i].state = READY;
            add_queue(&ready_queue[0], &tcb_tbl[i]);
            printf("ready:%p , tcb:%p\n",ready_queue,tcb_tbl[i].context);
        }
    }
    create_empty_tsk();
    add_queue(&ready_queue[1],&empty_tcb);
    schedule();
    EI(intsts);
}

void tsk_sleep(uint32_t ms){
    int intsts;
    DI(intsts);
    if(ms > 0 && cur_task != NULL){
        remove_queue(&ready_queue[cur_task->pri],cur_task);
        cur_task->wait_time = ms;
        add_queue(&wait_queue,cur_task);    
    }
    schedule();
    EI(intsts);
    
}