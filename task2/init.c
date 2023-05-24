#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define ARRAY_SIZE 300

int main() {
    key_t key;
    int shmid;
    int *shm_array;
    
    key = ftok("keyfile", 1);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    
    shmid = shmget(key, sizeof(int) * ARRAY_SIZE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    
    shm_array = (int*)shmat(shmid, NULL, 0);
    if (shm_array == (int*)-1) {
        perror("shmat");
        exit(1);
    }
    
    for (int i = 0; i < ARRAY_SIZE; i++) {
        shm_array[i] = 0;
    }
    
    shmdt(shm_array);
    
    printf("Shared memory segment created.\n");
    
    return 0;
}