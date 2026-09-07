#ifndef SHM_H
#define SHM_H

#include "types.h"

#define MAX_SHM_PAGES 10

struct shm_page {
  uint64 pa;
  int ref_count;
};

void shm_init(void);
uint64 shm_get(int id);

#endif
