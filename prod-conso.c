/*
 * Créer un programme "prod-conso.c" qui utilise la bibliothèque précédemment
 * créée et dont le but est le suivant : • Un process fils va produire une suite
 * d'entiers qu'il va stocker dans un buffer circulaire de taille 5 entiers. •
 * Le process père consomme les données de ce buffer circulaire. Bien entendu,
 * le process fils est bloqué lorsque le buffer est plein (il attend que le père
 * consomme les données et libère de la place), et le père est bloqué losque le
 * buffer est vide (il attend que le fils produise des données). Cette
 * synchronisation entre le père et le fils sera réalisée uniquement à l'aide de
 * deux sémaphores dont on aura soigneusement étudié les valeurs initiales.
 *  Indice :
 *  Votre programme réalisera donc les fonctions suivantes :
 *  • Création d'un segment de mémoire partagée pouvant contenir 5 entiers. Ce
 * segment de mémoire partagée sera vu comme un buffer circulaire géré par deux
 * "index" : un index d'écriture et un index de lecture qui évolueront
 * "circulairement" (0 1 2 3 4 0 1 2 ...). • Création d'un process "fils"
 * partageant le segment précédemment créé avec son "père". • Le process fils va
 * jouer le rôle du producteur. Il génère les entiers de 1 à 50 qu'il stocke
 * dans le buffer circulaire au fur et à mesure qu'il y a de la place libre. Il
 * gère donc l'index d'écriture du buffer circulaire. Le process père,
 * consommateur, va lire et afficher les données présentes dans le buffer
 * circulaire. Il gère l'index de lecture.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#include "CircularBuffer.h"
#include "sem_pv.h"
// gcc prod-conso.c -o prod-conso -L. -lsempv && ./prod-conso

key_t key = 42;
int sizeOfInt = sizeof(int);

int* init(int shm_id) {
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    int* adresse = shmat(shm_id, NULL, 0);
    if (adresse == (void*)-1) {
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

int* initSingleIntShm(int shm_id){
    if (shm_id == -1) {
        perror("shmget");
        exit(1);
    }

    int* adresse = shmat(shm_id, NULL, 0);
    if (adresse == (void*)-1) {
        perror("shmat");
        exit(1);
    }
    adresse[0] = 0;
    return adresse;
}

void closeShm(int* adresse, int shm_id) {
    if (shmdt(adresse) == -1) {
        perror("shmdt");
        exit(1);
    }

    if (shmctl(shm_id, IPC_RMID, NULL) == -1) {
        perror("shmctl");
        exit(1);
    }
}

int main() {
    int i, j = 0;

    int shm_id = shmget(key, 5*sizeOfInt, 0666 | IPC_CREAT);
    int shm_head = shmget(key + 31, sizeOfInt, 0666 | IPC_CREAT);
    int shm_tail = shmget(key + 32, sizeOfInt, 0666 | IPC_CREAT);

    int* adresse = init(shm_id);
    int* head = initSingleIntShm(shm_head);
    int* tail = initSingleIntShm(shm_tail);
    init_semaphore();
    val_sem(1, 4);
    val_sem(2, 0);
    int pid = fork();
    // We link the shared memory to the circular buffer manager
    CircularBuffer* circBuf = malloc(sizeof(CircularBuffer));
    initCircularBuffer(circBuf, adresse, 7);
    circBuf->head = head;
    circBuf->tail = tail;
    if (pid == 0) {
        for (i = 0; i < 50; i++) {
            P(1);
            // We write in the circular buffer
            pushToCircularBuffer(circBuf, i);
            printf("SON %d\n", i);
            // print the whole buffer
            V(2);
        }
        exit(0);
    } else {
        for (i = 0; i < 50; i++) {
            P(2);
            // We read from the circular buffer
            popFromCircularBuffer(circBuf, &j);
            printf("FATHER %d\n", j);
            V(1);
        }
        exit(0);
    }
    closeShm(adresse, shm_id);
    return 0;
}
