# Operating Systems Lab Assignment - 5
**Roll Number:** 2401MC58
**Name:** Mahesh

## Folder Structure & Implementation Details
- `Q1/`: Mutual Exclusion using Peterson's Algorithm and Kernel Shared Memory (`shm_get`).
- `Q2/`: Producer-Consumer Problem using Kernel Semaphore primitives (`sem_wait`, `sem_post`).
- `Q3/`: Readers-Writers Problem using Kernel Shared Memory and Semaphore primitives.
- `Q4/`: Dining Philosophers Problem with Asymmetric Fork Selection for Deadlock Avoidance.

## How to Build & Test
1. Copy required kernel files to xv6 `kernel/`.
2. Copy user programs (`peterson.c`, `prodcons.c`, `readwrite.c`, `dining.c`) to xv6 `user/`.
3. Apply additions from `Makefile_additions.txt` to xv6 `Makefile`.
4. Build and run via `make qemu`.
