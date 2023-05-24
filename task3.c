#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define SEM_KEY 1234
#define SHM_KEY 5678
#define NUM_CHILDREN 2
#define NUM_ITERATIONS 10000

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

void P(int sem_id) {
    struct sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = -1;
    sem_op.sem_flg = 0;
    semop(sem_id, &sem_op, 1);
}

void V(int sem_id) {
    struct sembuf sem_op;
    sem_op.sem_num = 0;
    sem_op.sem_op = 1;
    sem_op.sem_flg = 0;
    semop(sem_id, &sem_op, 1);
}

int main() {
    int shmid;
    int semid;
    int *shm_ptr;

    shmid = shmget(SHM_KEY, sizeof(int), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }
    

    shm_ptr = (int*)shmat(shmid, NULL, 0);
    if (shm_ptr == (int*)-1) {
        perror("shmat");
        exit(1);
    }
    
    semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }
    
    union semun sem_init;
    sem_init.val = 1;
    if (semctl(semid, 0, SETVAL, sem_init) == -1) {
        perror("semctl");
        exit(1);
    }
    
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();
        
        if (pid == -1) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
       
            for (int j = 0; j < NUM_ITERATIONS; j++) {
                P(semid); 
                
                (*shm_ptr)++; 
                
                V(semid); 
            }
            
            exit(0);
        }
    }
    
    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL);
    }
    
    printf("Final value: %d\n", *shm_ptr);
    
    if (shmdt(shm_ptr) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }

    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("semctl");
        exit(1);
    }
    
    return 0;
}
