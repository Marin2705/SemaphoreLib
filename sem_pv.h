#pragma once

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#define N_SEM 5

static int semid = -1;

int init_semaphore();

int detruire_semaphore();

// 3. int val_sem(int sem, int val)
//     Attribue la valeur "val" au sémaphore "sem" du groupe de sémaphores créé par
//                        "init_semaphore".Retourne la valeur de retour de "semctl" en cas de réussite,
//     -1 si "init_semaphore" n'a pas été appelé avant, -2 si le numéro de sémaphore est incorrect.

int val_sem(int sem, int val);

// 4. int P(int sem)
//     Réalise l'opération (P) sur le sémaphore numéro "sem" du groupe de sémaphores créé par "init_semaphore".Retourne la valeur de retour de "semop" en cas de réussite,
//     -1 si "init_semaphore" n'a pas été appelé avant, -2 si le numéro de sémaphore est incorrect.

int P(int sem);

// 5. int V(int sem)
//     Réalise l'opération (V) sur le sémaphore numéro "sem" du groupe de sémaphores créé par "init_semaphore".Retourne la valeur de retour de "semop" en cas de réussite,
//     -1 si "init_semaphore" n'a pas été appelé avant, -2 si le numéro de sémaphore est incorrect.

int V(int sem);
