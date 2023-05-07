#include "./libprocesslab/libprocesslab.h"

#define m 1000000

#define nb 4

long somme[nb];


void* contribution(void*p)
{
    printf("%d\n", *(int*)p);
    int no = *((int*)p);
    long firstNumber = (no * m/nb) + 1;
    long lastNumber = (no + 1) * m/nb;
    somme[no] = 0;
    for (long i = firstNumber; i <= lastNumber; i++) {
        somme[no] += i;
    }
    pthread_exit(NULL);
    return NULL;
}


void question2( )
{
    pthread_t tid[nb];
    int numbers[nb];
    for (int i = 0; i < nb; i++) {
        numbers[i] = i;
    }
    for (long i = 0; i < nb; i++) {
        pthread_create(&tid[i], NULL, contribution, &numbers[i]);
    }
    for (int i = 0; i < nb; i++) {
        pthread_join(tid[i], NULL);
    }

    long expectedResult = (long)m * ((long)m + 1) / 2;
    long total = 0;
    for (int i = 0; i < nb; i++) {
        total += somme[i];
    }
    printf("Somme: %ld,\t Résultat attendu: %ld", total, expectedResult);
}

