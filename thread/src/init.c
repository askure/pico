#include "../include/kernerl.h"

#define SYST_CSR 0xE000E010
#define SYST_RVR 0xE000E014
#define SYST_CVR 0xE000E018
void init(){
    // timer 初期化
    out_w(SYST_CSR, 6);
    out_w(SYST_RVR,1250000-1); // 10ms
    out_w(SYST_CVR,0); 
    out_w(SYST_CSR, 7); // タイマー割り込み設定

    for(int i=0; i< TCB_NUM; i++){
        tcb_tbl[i].state = NONEXIT;
    }
}