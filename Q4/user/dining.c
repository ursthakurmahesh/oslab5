#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_PHILOSOPHERS 5
#define CYCLES 5

struct dining_shm {
    int forks[NUM_PHILOSOPHERS];
};

void philosopher(struct dining_shm *shm, int id) {
    int left = id;
    int right = (id + 1) % NUM_PHILOSOPHERS;

    for (int c = 1; c <= CYCLES; c++) {
        printf("Philosopher %d: THINKING\n", id);
        sleep(2);

        printf("Philosopher %d: HUNGRY\n", id);

        // Asymmetric Lock Ordering to prevent Deadlock
        if (id % 2 == 0) {
            sem_wait(shm->forks[left]);
            sem_wait(shm->forks[right]);
        } else {
            sem_wait(shm->forks[right]);
            sem_wait(shm->forks[left]);
        }

        printf("Philosopher %d: EATING (Cycle %d/%d)\n", id, c, CYCLES);
        sleep(2);

        sem_post(shm->forks[left]);
        sem_post(shm->forks[right]);
    }

    printf("Philosopher %d: Completed cycles.\n", id);
    exit(0);
}

int main(int argc, char *argv[]) {
    struct dining_shm *shm = (struct dining_shm *)shm_get(3);
    if ((uint64)shm == 0) {
        printf("shm_get failed\n");
        exit(1);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        shm->forks[i] = sem_create(1);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        if (fork() == 0) {
            philosopher(shm, i);
        }
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        wait(0);
    }

    printf("Dining Philosophers completed without deadlock!\n");
    exit(0);
}
