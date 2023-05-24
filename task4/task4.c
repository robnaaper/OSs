#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define SEM_KEY 1234
#define NUM_SMOKERS 3

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

void smoker_process(int semid, char smoker_item, char match, char paper, char tobacco) {
    while (1) {
        P(semid);
        
        if (smoker_item != match && match != 0) {
            printf("Smoker with item %c: Found match. Smoking...\n", smoker_item);
            V(semid);
            sleep(1);
            printf("Smoker with item %c: Done smoking. Signaling barman.\n", smoker_item);
            printf("%c\n", match);
            exit(0);
        } else if (smoker_item != paper && paper != 0) {
            printf("Smoker with item %c: Found paper. Smoking...\n", smoker_item);
            V(semid);
            sleep(1);
            printf("Smoker with item %c: Done smoking. Signaling barman.\n", smoker_item);
            printf("%c\n", paper);
            exit(0);
        } else if (smoker_item != tobacco && tobacco != 0) {
            printf("Smoker with item %c: Found tobacco. Smoking...\n", smoker_item);
            V(semid);
            sleep(1);
            printf("Smoker with item %c: Done smoking. Signaling barman.\n", smoker_item);
            printf("%c\n", tobacco);
            exit(0);
        }
        
        V(semid);
        sleep(1);
    }
}

void barman_process(int semid) {
    char items[NUM_SMOKERS] = { 't', 'p', 'm' };
    int index = 0;
    
    while (1) {
        printf("Barman: Looking for item %c...\n", items[index]);
        P(semid);
        
        printf("Barman: Found item %c. Giving it to the smoker.\n", items[index]);
        char smoker_item = items[index];
        items[index] = 0;
        
        V(semid);
        
        sleep(1);
        
        index = (index + 1) % NUM_SMOKERS;
        
        printf("Barman: Waiting for smoker to finish smoking.\n");
        wait(NULL);
        
        printf("Barman: Smoker with item %c finished smoking.\n", smoker_item);
    }
}

int main() {
    int semid;
    
    semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        exit(1);
    }
    
    