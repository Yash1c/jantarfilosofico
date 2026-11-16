#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define T 8
#define M 200000

volatile long count = 0;
sem_t sem;

void *tarefa(void *arg)
{
    int i;
    for (i = 0; i < M; i++) {
        sem_wait(&sem);       // adquirir
        count = count + 1;
        sem_post(&sem);       // liberar
    }
    return NULL;
}

int main()
{
    pthread_t t[T];
    struct timespec ini, fim;
    double tempo;
    int i;

    sem_init(&sem, 0, 1);   // 1 permissao = binario

    clock_gettime(CLOCK_MONOTONIC, &ini);

    for (i = 0; i < T; i++)
        pthread_create(&t[i], NULL, tarefa, NULL);

    for (i = 0; i < T; i++)
        pthread_join(t[i], NULL);

    clock_gettime(CLOCK_MONOTONIC, &fim);
    tempo = (fim.tv_sec - ini.tv_sec) + (fim.tv_nsec - ini.tv_nsec) / 1e9;

    printf("esperado = %ld, obtido = %ld, tempo = %.4f s\n", (long)T * M, count, tempo);

    sem_destroy(&sem);
    return 0;
}
