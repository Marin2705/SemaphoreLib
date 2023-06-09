#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sem_pv.h"
// gcc prod-conso.c -o prod-conso -L. -lsempv && ./prod-conso

key_t key = 42;
int sizeOfInt = sizeof(int);

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
    adresse[1] = 0;
    adresse[2] = 0;
    adresse[3] = 0;
    adresse[4] = 0;
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

int main(){
    int shm_id = shmget(key, 64, 0666 | IPC_CREAT);
    int* adresse = init(shm_id);
    int indexRead = 0;
    int indexWrite = 0;
    init_semaphore();
    val_sem(1, 5);
    val_sem(2, 0);
    // val_sem(2, 5);
    if (fork() == 0){
        for (int i = 1; i <= 50; ++i){
            P(1); // bloqué si plein
            printf("CHILD %d\n", i);
            adresse[indexWrite++ % 5] = i;
            V(2);
        }
        exit(0);
    }
    for (int i = 0; i < 100; ++i){
        // bloqué si vide
        P(2);
        printf("FATHER %d\n", adresse[indexRead++ % 5]);
        V(1);
    }
    closeShm(adresse, shm_id);
    return 0;
}
