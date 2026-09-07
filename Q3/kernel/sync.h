#ifndef SYNC_H
#define SYNC_H

#include "spinlock.h"

#define MAX_SEMAPHORES 10

struct ksem {
  int value;
  int active;
  struct spinlock lock;
};

void sem_sys_init(void);
int sem_create(int init_val);
int sem_wait(int sem_id);
int sem_post(int sem_id);

#endif
