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
int port;
int tsk_id;
}TCB;

typedef struct data{
    uint32_t data;
    uint32_t ok;
}Data;

typedef struct{
    uint32_t r_[8]; // r4-r11（自動で退避しないレジスタ)
    uint32_t r[4];
    uint32_t r12; 
    uint32_t lr;
    uint32_t pc; // PC
    uint32_t xPSR; // xpsr
} StackFrame;

#define READY_NUM 10
#define READY_DEFALUT 5
extern TCB *cur_task;
extern TCB *next_task;
extern TCB *ready_queue[READY_NUM];
extern TCB tcb_tbl[TCB_NUM];
extern TCB *wait_queue;
extern int pending_dispatch;
extern Data user_data[TCB_NUM];
extern void add_queue(TCB ** , TCB *);
extern int remove_queue(TCB ** , TCB *);