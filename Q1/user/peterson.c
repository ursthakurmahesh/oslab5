#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

struct peterson_shm {
    volatile int flag[2];
    volatile int turn;
    volatile int shared_counter;
};

int main(int argc, char *argv[]) {
    struct peterson_shm *shm = (struct peterson_shm *)shm_get(0);
    if ((uint64)shm == 0) {
        printf("shm_get failed\n");
        exit(1);
    }

    shm->flag[0] = 0;
    shm->flag[1] = 0;
    shm->turn = 0;
    shm->shared_counter = 0;

    int pid = fork();
    if (pid < 0) {
        printf("fork failed\n");
        exit(1);
    }

    int self = (pid == 0) ? 1 : 0;
    int other = 1 - self;

    for (int i = 0; i < 10; i++) {
        // Entry Section
        shm->flag[self] = 1;
        shm->turn = other;
        __sync_synchronize();

        while (shm->flag[other] == 1 && shm->turn == other) {
            // Busy wait
        }

        // Critical Section
        int temp = shm->shared_counter;
        sleep(1);
        shm->shared_counter = temp + 1;
        printf("Process %d in CS, counter = %d\n", self, shm->shared_counter);

        // Exit Section
        shm->flag[self] = 0;
        __sync_synchronize();

        // Remainder Section
        sleep(1);
    }

    if (pid > 0) {
        wait(0);
        printf("Final Shared Counter Value = %d (Expected: 20)\n", shm->shared_counter);
    }

    exit(0);
}
