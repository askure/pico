#include "../include/kernerl.h"

void defalutEnd(){
    printf("End\n");
}

void * make_context(void * stackaddr, int32_t stacksize, void (*fp)(),void (*end)()){
    StackFrame *sfp = (StackFrame*)((uint32_t)stackaddr + stacksize);
    sfp--; 
    sfp-> xPSR = 0x01000000;
    sfp-> lr = (uint32_t)end;
    printf("make_lr:%p\n", sfp-> lr);
    sfp-> pc = (uint32_t)fp & ~1; 
    if(end == NULL){
        sfp-> lr = (uint32_t)defalutEnd;
    }
    return (void*)sfp;
}


