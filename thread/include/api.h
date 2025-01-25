#ifndef __API_H__
#define __API_H__
#include "def.h"

int create_tsk(void * stackaddr, int32_t stacksize, void (*fp)(),void (*end)());
void init();
void tsk_run();
void tsk_exit();
#endif