#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

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
    
    shmid = shmget(key, 0, 0);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    
    shm_array = (int*)shmat(shmid, NULL, 0);
    if (shm_array == (int*)-1) {
        perror("shmat");
        exit(1);
    }
    
    // Set up the sieve of Eratosthenes
    for (int i = 2; i < ARRAY_SIZE; i++) {
        if (shm_array[i] == 0) {
            for (int j = i * 2; j < ARRAY_SIZE; j += i) {
                shm_array[j] = 1;
            }
        }
    }
    
    shmdt(shm_array);
    
    printf("Sieve of Eratosthenes completed.\n");
    
    return 0;
}