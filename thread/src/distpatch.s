#include "pico.h"
#include "pico/asm_helper.S"
  .syntax unified
  .cpu cortex-m0plus
  .fpu softvfp
  .thumb_func
  .globl isr_pendsv

isr_pendsv:
    CPSID i  /*割込み禁止*/
    ldr   r0, =is_dispatch
    movs  r1, #1
    str   r1,[r0]

    /*レジスタ退避*/
    push    {r4-r7} 
    mov     r0, r8
    mov     r1, r9
    mov     r2, r10
    mov     r3, r11
    push    {r0-r3} 
    
    /*cur_taskの処理*/
    ldr r0 ,=cur_task
    ldr r1 ,[r0]
    cmp r1, #0
    beq disp_010
    /*今のspを,cur_task->contextに格納*/
    mov r2 ,sp
    str r2 ,[r1]
disp_010:
    ldr r1 ,=next_task
    ldr r2 ,[r1]
/*    cmp r2 ,#0
    bne disp_030
    r1だけ破壊しないように
    str r2,[r0]
disp_020: 
    CPSIE i
    nop
    CPSID i
    ldr r2,[r1]
    cmp r2 , #0
    beq disp_020*/

disp_030: 
    str r2 ,[r0]
    ldr r3, [r2]
    mov sp ,r3
    /*レジスタ復帰*/
    pop	{r0-r3}
    mov	r11, r3
    mov	r10, r2
    mov	r9, r1
    mov	r8, r0
    pop	{r4-r7}

    ldr   r0, =is_dispatch
    movs   r1, #0
    str   r1,[r0]
    CPSIE i

    bx	lr

  .syntax unified
  .cpu cortex-m0plus
  .fpu softvfp
  .thumb_func
  .globl isr_systick

isr_systick:
    /*レジスタ退避*/
    /*memo:psr,pc,lr,r12,r3,r2,r1,r0は退避済み*/ 

    push    {r4-r7} 
    mov     r0, r8
    mov     r1, r9
    mov     r2, r10
    mov     r3, r11
    push    {r0-r3} 

    ldr r0 ,=cur_task
    ldr r1 ,[r0]
    cmp r1, #0
    beq disp_040
    mov r2 ,sp
    str r2 ,[r1]
disp_040:
    movs r0,#1
    mov r4,lr
    push {r4}
    bl test_print
    pop {r4}
    mov lr,r4

    ldr r0 ,=cur_task
    ldr r1,[r0]
    cmp r1,#0
    beq disp_050
    ldr r2,[r1]
    mov sp ,r2
disp_050:   
    mov r0,sp
    mov r4,lr
    push {r4}
    bl print
    pop {r4}
    mov lr,r4

    pop	{r0-r3}
    mov	r11, r3
    mov	r10, r2
    mov	r9, r1
    mov	r8, r0
    pop	{r4-r7}

    bx lr