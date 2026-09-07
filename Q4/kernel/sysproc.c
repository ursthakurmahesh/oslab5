#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "shm.h"
#include "sync.h"

uint64 sys_shm_get(void) { int id; argint(0, &id); return shm_get(id); }
uint64 sys_sem_create(void) { int val; argint(0, &val); return sem_create(val); }
uint64 sys_sem_wait(void) { int id; argint(0, &id); return sem_wait(id); }
uint64 sys_sem_post(void) { int id; argint(0, &id); return sem_post(id); }
