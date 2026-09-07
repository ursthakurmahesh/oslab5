#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUFFER_SIZE 5
#define TOTAL_ITEMS 20

struct prodcons_shm {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
};

int main(int argc, char *argv[]) {
    struct prodcons_shm *shm = (struct prodcons_shm *)shm_get(1);
    if ((uint64)shm == 0) {
        printf("shm_get failed\n");
        exit(1);
    }

    shm->in = 0;
    shm->out = 0;

    int empty = sem_create(BUFFER_SIZE);
    int full = sem_create(0);
    int mutex = sem_create(1);

    int pid = fork();
    if (pid == 0) {
        // Consumer
        for (int i = 1; i <= TOTAL_ITEMS; i++) {
            sem_wait(full);
            sem_wait(mutex);

            int item = shm->buffer[shm->out];
            printf("Consumer [PID %d]: Consumed item %d from slot %d\n", getpid(), item, shm->out);
            shm->out = (shm->out + 1) % BUFFER_SIZE;

            sem_post(mutex);
            sem_post(empty);
            sleep(2);
        }
        exit(0);
    } else {
        // Producer
        for (int i = 1; i <= TOTAL_ITEMS; i++) {
            sem_wait(empty);
            sem_wait(mutex);

            shm->buffer[shm->in] = i;
            printf("Producer [PID %d]: Produced item %d at slot %d\n", getpid(), i, shm->in);
            shm->in = (shm->in + 1) % BUFFER_SIZE;

            sem_post(mutex);
            sem_post(full);
            sleep(1);
        }
        wait(0);
    }
    exit(0);
}
