#define PRIORITY_NUM 16
#define TCB_NUM 32
#define NULL ((void *)0)

enum STATE{
    NONEXIT,
    READY,
    WAIT,
    DORAMENT
};

typedef struct st_tcb{
void *context; // アセンブリで参照するから，ここは変えてはダメ
struct st_tcb *pre;
struct st_tcb *next;
enum STATE state;
void * stackaddr;
int32_t stacksize; 
void (*fp)();
void (*end)();
uint32_t wait_time;
int priority;
int tsk_id;
}TCB;

typedef struct{
    uint32_t r_[8]; // r4-r11（自動で退避しないレジスタ)
    uint32_t r[4];
    uint32_t r12; 
    uint32_t lr;
    uint32_t pc; // PC
    uint32_t xPSR; // xpsr
} StackFrame;


extern TCB *cur_task;
extern TCB *next_task;
extern TCB *ready_queue[2];
extern TCB tcb_tbl[TCB_NUM];
extern TCB *wait_queue;

extern void add_queue(TCB ** , TCB *);
extern void remove_queue(TCB ** , TCB *);