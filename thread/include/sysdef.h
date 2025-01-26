#ifndef SYSDIF_H
#define SYSDIF_H
// 割り込み禁止
#define	DI(intsts)	__asm__ volatile("CPSID i");

#define	EI(intsts)	__asm__ volatile("CPSIE i");

static inline void set_primask( int pm )
{
    __asm__ volatile("msr primask, %0":: "r"(pm));
}

static inline uint32_t get_primask( void )
{
    uint32_t  pm;
    __asm__ volatile("mrs %0, primask": "=r"(pm));
    return pm;
}
static inline void out_w(uint32_t addr, uint32_t data){
    *(volatile uint32_t*)addr = data;
}
#endif