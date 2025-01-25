#ifndef SYSDIF_H
#define SYSDIF_H
// 割り込み禁止
#define	DI(intsts)	(intsts=get_primask(), set_primask(1))

#define	EI(intsts)	(set_primask(intsts))

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