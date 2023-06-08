#include <unistd.h>
#include "sem_pv.h"

// 1. Appel à "init_semaphore"
// 2. Appel à "val_sem(2,1)"
// 3. Appel à "P(2)"
// 4. Attente de 30 secondes
// 5. Appel à "V(2)"
// 6. Appel à "detruire_semaphore"

int main()
{
    printf("A");
    fflush(stdout);
    init_semaphore();
    printf("B");
    fflush(stdout);
    val_sem(2, 1); // 0 pour bloquer le sem et tester
    printf("C");
    fflush(stdout);
    P(2);
    printf("D");
    fflush(stdout);
    sleep(3);
    printf("E");
    fflush(stdout);
    V(2);
    printf("F");
    fflush(stdout);
    detruire_semaphore();
    printf("G");
    fflush(stdout);
    return 0;
}