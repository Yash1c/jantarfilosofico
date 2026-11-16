
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5

sem_t garfo[N];

void pensar(int p)
{
    printf("Filosofo %d esta pensando\n", p);
    usleep(rand() % 500000);
}

void comer(int p)
{
    printf("Filosofo %d esta comendo\n", p);
    usleep(rand() % 500000);
}

void *filosofo(void *arg)
{
    int p = *(int *)arg;
    int esq = p;             // garfo i fica entre filosofo i e (i+1) mod N
    int dir = (p + 1) % N;

    while (1) {
        pensar(p);

        printf("Filosofo %d esta com fome\n", p);

        sem_wait(&garfo[esq]);
        printf("Filosofo %d pegou o garfo %d (esquerda)\n", p, esq);

        sleep(1);

        printf("Filosofo %d esperando o garfo %d (direita)\n", p, dir);
        sem_wait(&garfo[dir]);
        printf("Filosofo %d pegou o garfo %d (direita)\n", p, dir);

        comer(p);

        sem_post(&garfo[dir]);
        sem_post(&garfo[esq]);
    }
    return NULL;
}

int main()
{
    pthread_t t[N];
    int id[N];
    int i;

    setvbuf(stdout, NULL, _IONBF, 0);

    for (i = 0; i < N; i++)
        sem_init(&garfo[i], 0, 1);

    for (i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&t[i], NULL, filosofo, &id[i]);
    }

    for (i = 0; i < N; i++)
        pthread_join(t[i], NULL);

    return 0;
}
