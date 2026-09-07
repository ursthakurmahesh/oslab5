extern uint64 sys_shm_get(void);
extern uint64 sys_sem_create(void);
extern uint64 sys_sem_wait(void);
extern uint64 sys_sem_post(void);

// Syscall entries:
// [SYS_shm_get]    sys_shm_get,
// [SYS_sem_create] sys_sem_create,
// [SYS_sem_wait]   sys_sem_wait,
// [SYS_sem_post]   sys_sem_post,
