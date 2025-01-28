#include "../include/kernerl.h"
int is_dispatch=0;
int count =0;
TCB *ready_queue[READY_NUM];
TCB *cur_task = NULL;
TCB *next_task = NULL;
// void schedule(){
//     int i=0;
//     // for(;i<2;i++) if(ready_queue[i] != NULL) break;
//     // next_task = ready_queue[i];
// }