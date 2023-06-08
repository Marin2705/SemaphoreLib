#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sem_pv.h"
// gcc excl-mutu.c -o excl-mutu -L. -lsempv && ./excl-mutu

key_t key = 42;

int* init(int shm_id){
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    int* adresse = shmat(shm_id, NULL, 0);
    if (adresse == (void*) -1) {
        perror("shmat");
        exit(1);
    }
    adresse[0] = 0;
    return adresse;
}

void closeShm(int* adresse, int shm_id){
    if (shmdt(adresse) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
}

int run(int* adresse){
    int A = adresse[0];
    printf("A is : %d\n",A);
    usleep(3);
    adresse[0] = A + 1;
    usleep(3);
    return adresse[0];
}

int main(){
    int shm_id = shmget(key, 64, 0666 | IPC_CREAT);
    int* adresse = init(shm_id);
    int localA;
    init_semaphore();
    val_sem(1, 1);
    if (fork() == 0){
        for (int i = 0; i < 100; ++i){
            P(1);
            printf("CHILD %d\n", i);
            localA = run(adresse);
            V(1);
        }
        printf("final value (in child) is : %d\n",localA);
        exit(0);
    }
    for (int i = 0; i < 100; ++i){
        P(1);
        localA = run(adresse);
        V(1);
    }
    printf("final value (in  father) is : %d\n",localA);
    closeShm(adresse, shm_id);
    return 0;
}
