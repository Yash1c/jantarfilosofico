

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t LOCK_A = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t LOCK_B = PTHREAD_MUTEX_INITIALIZER;

void *thread1(void *arg)
{
    pthread_mutex_lock(&LOCK_A);
    printf("T1 pegou LOCK_A\n");

    usleep(50000); //

    printf("T1 esperando LOCK_B...\n");
    pthread_mutex_lock(&LOCK_B);
    printf("T1 concluiu\n");

    pthread_mutex_unlock(&LOCK_B);
    pthread_mutex_unlock(&LOCK_A);
    return NULL;
}

void *thread2(void *arg)
{
    pthread_mutex_lock(&LOCK_B);
    printf("T2 pegou LOCK_B\n");

    usleep(50000);

    printf("T2 esperando LOCK_A...\n");
    pthread_mutex_lock(&LOCK_A);
    printf("T2 concluiu\n");

    pthread_mutex_unlock(&LOCK_A);
    pthread_mutex_unlock(&LOCK_B);
    return NULL;
}

int main()
{
    pthread_t t1, t2;

    setvbuf(stdout, NULL, _IONBF, 0);
    printf("PID = %d\n", getpid());

    pthread_create(&t1, NULL, thread1, NULL);
    pthread_create(&t2, NULL, thread2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("fim (se chegou aqui nao travou)\n");
    return 0;
}
