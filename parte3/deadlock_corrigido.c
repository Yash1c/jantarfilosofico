

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t LOCK_A = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t LOCK_B = PTHREAD_MUTEX_INITIALIZER;

void *tarefa(void *arg)
{
    int id = *(int *)arg;

    pthread_mutex_lock(&LOCK_A);
    printf("T%d pegou LOCK_A\n", id);

    usleep(50000); // 

    pthread_mutex_lock(&LOCK_B);
    printf("T%d pegou LOCK_B\n", id);

    // secao critica
    printf("T%d concluiu\n", id);

    pthread_mutex_unlock(&LOCK_B);
    pthread_mutex_unlock(&LOCK_A);
    return NULL;
}

int main()
{
    pthread_t t1, t2;
    int id1 = 1, id2 = 2;

    setvbuf(stdout, NULL, _IONBF, 0);

    pthread_create(&t1, NULL, tarefa, &id1);
    pthread_create(&t2, NULL, tarefa, &id2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("fim, sem deadlock\n");
    return 0;
}
