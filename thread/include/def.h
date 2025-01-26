
#ifndef __DEF_H__
#define __DEF_H__
#include <stdint.h>
#include "sysdef.h"
#include "pico/time.h"

extern void isr_pendsv();
extern void isr_systick(void);
extern void isr_hardfault();
extern void distpatch_test();
extern void *make_context();
extern void schedule();


#define SCB_ICSR        0xE000ED04      // 割込み制御ステートレジスタのアドレス
#define ICSR_PENDSVSET  (1<<28)         // PendSV set-pending ビット
static inline void dispatch( void )
{   
    out_w(SCB_ICSR,ICSR_PENDSVSET);    // PendSV例外を発生
}


#endif