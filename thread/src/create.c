#include "../include/kernerl.h"
TCB tcb_tbl[TCB_NUM];
TCB empty_tcb;
uint32_t *stack_empty[256/sizeof(uint32_t)];
void empty_task(){ 
    while(1)
    printf("empty\n");
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
        tcb_tbl[i].priority = READY_DEFALUT;
        tcb_tbl[i].pre = NULL;
        tcb_tbl[i].next = NULL;
        tcb_tbl[i].port = i;
    }
    EI(intsts);
    return i;
}
void make_empty_task(){
    int intsts;
    DI(intsts);
    empty_tcb.context = make_context(stack_empty, sizeof(stack_empty), empty_task, NULL);
    empty_tcb.state = READY;
    empty_tcb.priority = READY_NUM-1;
    empty_tcb.pre = NULL;
    empty_tcb.next = NULL;
    EI(intsts);
}

void tsk_run(){
    int intsts;
    DI(intsts);
    for(int i=0 ; i< TCB_NUM; i++){
        if(tcb_tbl[i].state == DORAMENT){
            tcb_tbl[i].state = READY;
            add_queue(&ready_queue[tcb_tbl[i].priority], &tcb_tbl[i]);
        }
    }
    make_empty_task();
    add_queue(&ready_queue[empty_tcb.priority], &empty_tcb);
    EI(intsts);
}


void tsk_sleep(uint32_t ms){
    while(pending_dispatch);
    int intsts;
    // printf("tsk_sleep;tskid%d\n",cur_task->tsk_id);
    DI(intsts);
    if(ms > 0 && cur_task != NULL){
        int err = remove_queue(&ready_queue[cur_task->priority],cur_task);
        if(err) {
            EI(intsts);
            return;
        }
        cur_task->wait_time = ms;
        add_queue(&wait_queue,cur_task);    
    }
    pending_dispatch = 1; 
    EI(intsts);
    
}
