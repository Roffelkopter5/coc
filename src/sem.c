#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>

#include "sem.h"

struct SEM
{
    volatile int value;
    pthread_mutex_t mutex;
    pthread_cond_t condition;
};

SEM *semCreate(int initVal)
{
    SEM *sem = malloc(sizeof(SEM));
    if (sem == NULL)
    {
        return NULL;
    }
    sem->value = initVal;
    if (pthread_mutex_init(&sem->mutex, NULL) != 0)
    {
        return NULL;
    }
    if (pthread_cond_init(&sem->condition, NULL) != 0)
    {
        pthread_mutex_destroy(&sem->mutex);
        return NULL;
    }
    return sem;
}

void semDestroy(SEM *sem)
{
    if (sem == NULL)
    {
        return;
    }
    pthread_mutex_destroy(&sem->mutex);
    pthread_cond_destroy(&sem->condition);
    free(sem);
}

void P(SEM *sem)
{
    pthread_mutex_lock(&sem->mutex);
    while (sem->value <= 0)
    {
        pthread_cond_wait(&sem->condition, &sem->mutex);
    }
    sem->value -= 1;
    pthread_mutex_unlock(&sem->mutex);
}

void V(SEM *sem)
{
    pthread_mutex_lock(&sem->mutex);
    sem->value += 1;
    if (sem->value == 1)
    {
        pthread_cond_signal(&sem->condition);
    }
    pthread_mutex_unlock(&sem->mutex);
}
