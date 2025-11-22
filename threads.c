#include "philo.h"

typedef struct
{
    char *name;
    pthread_t thread;
    pthread_mutex_t ***forks;
} t_p;

void *live(void *arg)
{
    t_p *philo = (t_p *)arg;
    printf("%s picked up fork\n", philo->name);
    pthread_mutex_lock(*philo->forks[0]);
    printf("%s picked up fork\n", philo->name);
    pthread_mutex_lock(*philo->forks[1]);
    printf("%s is eating\n", philo->name);
    usleep(200000);
    printf("%s is sleeping\n", philo->name);
    pthread_mutex_unlock(*philo->forks[0]);
    pthread_mutex_unlock(*philo->forks[1]);
    return NULL;
}

void run_threads()
{
    t_p P1;
    t_p P2;

    pthread_mutex_t f1;
    pthread_mutex_t f2;
    pthread_mutex_t **forks = NULL;

    pthread_mutex_init(&f1, NULL);
    pthread_mutex_init(&f2, NULL);

    forks[0] = &f1;
    forks[1] = &f2;
    P1.forks = &forks;
    P2.forks = &forks;
    P1.name = "P1";
    P1.name = "P2";

    pthread_create(&P1.thread, NULL, live, (void *)&P1);
    pthread_create(&P2.thread, NULL, live, (void *)&P2);

    pthread_join(P1.thread, NULL);
    pthread_join(P2.thread, NULL);

    pthread_mutex_destroy(&f1);
    pthread_mutex_destroy(&f2);

    free(forks);
}