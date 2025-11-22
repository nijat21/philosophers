#include "philo.h"

typedef struct
{
    int counter;
    pthread_mutex_t lock;
} SharedData;

void *increment(void *arg)
{
    SharedData *data = (SharedData *)arg;
    pthread_mutex_lock(&data->lock);
    data->counter++;
    pthread_mutex_unlock(&data->lock);
    return NULL;
}

void run_threads()
{
    pthread_t t1;
    pthread_t t2;
    SharedData data;

    data.counter = 0;
    pthread_mutex_init(&data.lock, NULL);

    pthread_create(&t1, NULL, increment, (void *)&data);
    pthread_create(&t2, NULL, increment, (void *)&data);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Counter = %d\n", data.counter);

    pthread_mutex_destroy(&data.lock);
}