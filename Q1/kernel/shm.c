#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "shm.h"

static struct spinlock shm_lock;
static struct shm_page shm_pages[MAX_SHM_PAGES];

void
shm_init(void)
{
  initlock(&shm_lock, "shm_lock");
  for (int i = 0; i < MAX_SHM_PAGES; i++) {
    shm_pages[i].pa = 0;
    shm_pages[i].ref_count = 0;
  }
}

uint64
shm_get(int id)
{
  if (id < 0 || id >= MAX_SHM_PAGES)
    return 0;

  acquire(&shm_lock);
  if (shm_pages[id].pa == 0) {
    char *mem = kalloc();
    if (mem == 0) {
      release(&shm_lock);
      return 0;
    }
    memset(mem, 0, PGSIZE);
    shm_pages[id].pa = (uint64)mem;
  }
  shm_pages[id].ref_count++;
  uint64 pa = shm_pages[id].pa;
  release(&shm_lock);

  struct proc *p = myproc();
  uint64 va = PGROUNDUP(p->sz);

  if (mappages(p->pagetable, va, PGSIZE, pa, PTE_W | PTE_R | PTE_U) < 0) {
    return 0;
  }

  p->sz = va + PGSIZE;
  return va;
}
