// #include <sys/types.h>
// #include <sys/ipc.h>
// #include <sys/sem.h>
// #include <stdio.h>
// #define N_SEM 1
#include "sem_pv.h"

// union semun
// {
//     int val;
//     struct semid_ds *buf;
//     ushort *array;
// };

int init_semaphore()
{
    union semun arg;
    arg.val = 0;

    if (semid != -1)
    {
        printf("Semaphore already initialized\n");
        return -1;
    }
    if ((semid = semget(IPC_PRIVATE, N_SEM, 0666)) == -1)
    {
        perror("echec creation semaphore");
        return -2;
    }

    for (int i = 0; i < N_SEM; i++)
    {
        // tester echec
        semctl(semid, i, SETVAL, arg);
    }
    return 0;
}

int detruire_semaphore()
{
    if (semid == -1)
    {
        printf("Semaphore not initialized\n");
        return -1;
    }

    int ret = semctl(semid, 0, IPC_RMID, 0);
    semid = -1;
    return ret;
}

// 3. int val_sem(int sem, int val)
//     Attribue la valeur "val" au sémaphore "sem" du groupe de sémaphores créé par
//                        "init_semaphore".Retourne la valeur de retour de "semctl" en cas de réussite,
//     -1 si "init_semaphore" n'a pas été appelé avant, -2 si le numéro de sémaphore est incorrect.

int val_sem(int sem, int val)
{
    union semun arg;
    arg.val = val;

    if (semid == -1)
    {
        printf("Semaphore not initialized\n");
        return -1;
    }
    if (sem < 0 || sem > 4)
    {
        printf("Semaphore number incorrect\n");
        return -2;
    }
    int ret = semctl(semid, sem, SETVAL, arg);
    return ret;
}

// 4. int P(int sem)
//     Réalise l'opération (P) sur le sémaphore numéro "sem" du groupe de sémaphores créé par "init_semaphore".Retourne la valeur de retour de "semop" en cas de réussite,
//     -1 si "init_semaphore" n'a pas été appelé avant, -2 si le numéro de sémaphore est incorrect.

struct sembuf op_P = {-1, -1, 0};

struct sembuf op_V = {-1, 1, 0};

int P(int sem)
{
    if (semid == -1)
    {
        printf("Semaphore not initialized\n");
        return -1;
    }
    if (sem < 0 || sem > N_SEM)
    {
        printf("Semaphore number incorrect\n");
        return -2;
    }

    op_P.sem_num = sem;
    int ret = semop(semid, &op_P, 1);
    return ret;
}

// 5. int V(int sem)
//     Réalise l'opération (V) sur le sémaphore numéro "sem" du groupe de sémaphores créé par "init_semaphore".Retourne la valeur de retour de "semop" en cas de réussite,
//     -1 si "init_semaphore" n'a pas été appelé avant, -2 si le numéro de sémaphore est incorrect.

int V(int sem)
{
    if (semid == -1)
    {
        printf("Semaphore not initialized\n");
        return -1;
    }
    if (sem < 0 || sem > N_SEM)
    {
        printf("Semaphore number incorrect\n");
        return -2;
    }

    op_V.sem_num = sem;
    int ret = semop(semid, &op_V, 1);
    return ret;
}
