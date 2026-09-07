#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

struct rw_shm {
    int shared_data;
    int read_count;
    int mutex;
    int rw_lock;
};

void reader(struct rw_shm *shm, int id) {
    for (int i = 0; i < 3; i++) {
        sem_wait(shm->mutex);
        shm->read_count++;
        if (shm->read_count == 1) {
            sem_wait(shm->rw_lock);
        }
        sem_post(shm->mutex);

        printf("Reader %d [PID %d]: Read shared_data = %d (Active Readers = %d)\n",
               id, getpid(), shm->shared_data, shm->read_count);
        sleep(2);

        sem_wait(shm->mutex);
        shm->read_count--;
        printf("Reader %d [PID %d]: Finished reading. (Remaining Readers = %d)\n",
               id, getpid(), shm->read_count);
        if (shm->read_count == 0) {
            sem_post(shm->rw_lock);
        }
        sem_post(shm->mutex);

        sleep(2);
    }
    exit(0);
}

void writer(struct rw_shm *shm, int id) {
    for (int i = 0; i < 3; i++) {
        sem_wait(shm->rw_lock);

        shm->shared_data += 10;
        printf("Writer %d [PID %d]: >>> EXCLUSIVE WRITE <<< Updated shared_data to %d\n",
               id, getpid(), shm->shared_data);
        sleep(3);

        sem_post(shm->rw_lock);
        sleep(3);
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    struct rw_shm *shm = (struct rw_shm *)shm_get(2);
    if ((uint64)shm == 0) {
        printf("shm_get failed\n");
        exit(1);
    }

    shm->shared_data = 100;
    shm->read_count = 0;
    shm->mutex = sem_create(1);
    shm->rw_lock = sem_create(1);

    for (int i = 0; i < 3; i++) {
        if (fork() == 0) reader(shm, i + 1);
    }
    for (int i = 0; i < 2; i++) {
        if (fork() == 0) writer(shm, i + 1);
    }

    for (int i = 0; i < 5; i++) wait(0);

    printf("Readers-Writers complete. Final shared_data = %d\n", shm->shared_data);
    exit(0);
}
