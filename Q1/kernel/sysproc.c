#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "shm.h"

uint64
sys_shm_get(void)
{
  int id;
  argint(0, &id);
  return shm_get(id);
}
