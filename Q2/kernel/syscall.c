extern uint64 sys_sem_create(void);
extern uint64 sys_sem_wait(void);
extern uint64 sys_sem_post(void);

// In syscalls array:
// [SYS_sem_create] sys_sem_create,
// [SYS_sem_wait]   sys_sem_wait,
// [SYS_sem_post]   sys_sem_post,
