#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sync.h"

static struct ksem semaphores[MAX_SEMAPHORES];
static struct spinlock sem_table_lock;

void
sem_sys_init(void)
{
  initlock(&sem_table_lock, "sem_table");
  for (int i = 0; i < MAX_SEMAPHORES; i++) {
    semaphores[i].active = 0;
    semaphores[i].value = 0;
    initlock(&semaphores[i].lock, "sem");
  }
}

int
sem_create(int init_val)
{
  acquire(&sem_table_lock);
  for (int i = 0; i < MAX_SEMAPHORES; i++) {
    if (!semaphores[i].active) {
      semaphores[i].active = 1;
      semaphores[i].value = init_val;
      release(&sem_table_lock);
      return i;
    }
  }
  release(&sem_table_lock);
  return -1;
}

int
sem_wait(int id)
{
  if (id < 0 || id >= MAX_SEMAPHORES) return -1;
  struct ksem *s = &semaphores[id];

  acquire(&s->lock);
  while (s->value <= 0) {
    sleep(s, &s->lock);
  }
  s->value--;
  release(&s->lock);
  return 0;
}

int
sem_post(int id)
{
  if (id < 0 || id >= MAX_SEMAPHORES) return -1;
  struct ksem *s = &semaphores[id];

  acquire(&s->lock);
  s->value++;
  wakeup(s);
  release(&s->lock);
  return 0;
}
