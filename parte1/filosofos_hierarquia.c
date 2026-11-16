#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define N 5
#define TEMPO 20

sem_t garfo[N];
char *estado[N];
int refeicoes[N];
int rodando = 1;

void pensar(int p)
{
    usleep(rand() % 500000);
}

void comer(int p)
{
    usleep(rand() % 500000);
}

void *filosofo(void *arg)
{
    int p = *(int *)arg;
    int g1 = p;
    int g2 = (p + 1) % N;

    // left = menor, right = maior
    int left = g1 < g2 ? g1 : g2;
    int right = g1 > g2 ? g1 : g2;

    while (rodando) {
        pensar(p);

        estado[p] = "com fome";
        printf("Filosofo %d esta %s\n", p, estado[p]);

        sem_wait(&garfo[left]);
        sem_wait(&garfo[right]);

        estado[p] = "comendo";
        refeicoes[p]++;
        printf("Filosofo %d esta %s (garfos %d e %d) - ja comeu %d vezes\n",
               p, estado[p], left, right, refeicoes[p]);
        comer(p);

        sem_post(&garfo[right]);
        sem_post(&garfo[left]);

        estado[p] = "pensando";
        printf("Filosofo %d esta %s\n", p, estado[p]);
    }
    return NULL;
}

int main()
{
    pthread_t t[N];
    int id[N];
    int i;

    setvbuf(stdout, NULL, _IONBF, 0);
    srand(time(NULL));

    for (i = 0; i < N; i++) {
        sem_init(&garfo[i], 0, 1);
        estado[i] = "pensando";
        refeicoes[i] = 0;
    }

    for (i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&t[i], NULL, filosofo, &id[i]);
    }

    sleep(TEMPO);
    rodando = 0;

    for (i = 0; i < N; i++)
        pthread_join(t[i], NULL);

    printf("\n--- fim da simulacao (%d s) ---\n", TEMPO);
    for (i = 0; i < N; i++)
        printf("Filosofo %d comeu %d vezes\n", i, refeicoes[i]);

    return 0;
}
