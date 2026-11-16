#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define T 8
#define M 200000

volatile long count = 0;

void *tarefa(void *arg)
{
    int i;
    for (i = 0; i < M; i++)
        count = count + 1;
    return NULL;
}

int main()
{
    pthread_t t[T];
    struct timespec ini, fim;
    double tempo;
    int i;

    clock_gettime(CLOCK_MONOTONIC, &ini);

    for (i = 0; i < T; i++)
        pthread_create(&t[i], NULL, tarefa, NULL);

    for (i = 0; i < T; i++)
        pthread_join(t[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    tempo = (fim.tv_sec - ini.tv_sec) + (fim.tv_nsec - ini.tv_nsec) / 1e9;

    printf("esperado = %ld, obtido = %ld, tempo = %.4f s\n", (long)T * M, count, tempo);
    return 0;
}
