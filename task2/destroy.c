#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
    key_t key;
    int shmid;
    
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
    
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
    
    if (remove("keyfile") == -1) {
        perror("remove");
        exit(1);
    }
    
    printf("Shared memory segment marked for deletion.\n");
    
    return 0;
}