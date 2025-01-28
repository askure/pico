#include "../include/kernerl.h"
TCB tcb_tbl[TCB_NUM];
uint32_t *stack_empty[256/sizeof(uint32_t)];
void empty_task(){ 
    while(1);
}
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
        tcb_tbl[i].priority = 0;
        tcb_tbl[i].pre = NULL;
        tcb_tbl[i].next = NULL;
        tcb_tbl[i].tsk_id = i;
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
            int priority = tcb_tbl[i].priority;
            add_queue(&ready_queue[priority], &tcb_tbl[i]);
        }
    }
    TCB emptytask;
    make_empty_task(&emptytask,stack_empty,sizeof(stack_empty),empty_task,NULL);
    add_queue(&ready_queue[emptytask.priority], &emptytask);
    schedule();
    EI(intsts);
}
void make_empty_task(TCB * tcb,void * stackaddr, int32_t stacksize, void (*fp)(),void (*end)()){
    int intsts;
    DI(intsts);
    tcb->context = make_context(stackaddr, stacksize, fp, end);
    tcb->stackaddr = stackaddr;
    tcb->stacksize = stacksize;
    tcb->fp = fp;
    tcb->end = end;
    tcb->state = DORAMENT;
    tcb->priority = 1;
    tcb->pre = NULL;
    tcb->next = NULL;
    tcb->tsk_id = -1;
    EI(intsts);
}

void tsk_sleep(uint32_t ms){
    int intsts;
    // printf("tsk_sleep;tskid%d\n",cur_task->tsk_id);
    DI(intsts);
    if(ms > 0 && cur_task != NULL){
        remove_queue(&ready_queue[cur_task->priority],cur_task);
        cur_task = NULL;
        // cur_task->wait_time = ms;
        // add_queue(&wait_queue,cur_task);    
    }
    EI(intsts);
    
}
