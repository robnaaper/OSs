#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number>\n", argv[0]);
        exit(1);
    }
    
    int number = atoi(argv[1]);
    
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
    
    int is_prime = shm_array[number];
    
    shmdt(shm_array);
    
    if (is_prime)
        printf("Yes\n");
    else
        printf("No\n");
    
    return 0;
}