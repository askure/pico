#include "../include/kernerl.h"
typedef struct{
    uint32_t r_[8]; // r4-r11（自動で退避しないレジスタ)
    uint32_t r[4];
    uint32_t r12; 
    uint32_t lr;
    uint32_t pc; // PC
    uint32_t xPSR; // xpsr
} StackFrame;

void defalutEnd(){
    printf("End\n");
}

void * make_context(void * stackaddr, int32_t stacksize, void (*fp)(),void (*end)()){
    StackFrame *sfp = (StackFrame*)((uint32_t)stackaddr + stacksize);
    sfp--; 
    sfp-> xPSR = 0x01000000;
    sfp-> lr = (uint32_t)end;
    sfp-> pc = (uint32_t)fp & ~1; 
    if(end == NULL){
        sfp-> lr = (uint32_t)defalutEnd;
    }
    return (void*)sfp;
}


