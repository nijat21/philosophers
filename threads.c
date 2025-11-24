#include "philo.h"

typedef struct
{
    int n;
    int total;
    pthread_t thread;
    pthread_mutex_t *forks;
} t_p;

void *live(void *arg)
{
    t_p *philo = (t_p *)arg;
    int left_fork;
    int right_fork;

    right_fork = philo->n;
    left_fork = (philo->n + 1) % philo->total;

    if (philo->n % 2 == 0)
    {
        pthread_mutex_lock(&philo->forks[left_fork]);
        printf("P%d picked up left fork\n", philo->n + 1);
        pthread_mutex_lock(&philo->forks[right_fork]);
        printf("P%d picked up right fork\n", philo->n + 1);
    }
    else
    {
        pthread_mutex_lock(&philo->forks[right_fork]);
        printf("P%d picked up right fork\n", philo->n + 1);
        pthread_mutex_lock(&philo->forks[left_fork]);
        printf("P%d picked up left fork\n", philo->n + 1);
    }

    printf("P%d is eating\n", philo->n + 1);
    usleep(2000000);

    printf("P%d is sleeping\n", philo->n + 1);

    pthread_mutex_unlock(&philo->forks[left_fork]);
    pthread_mutex_unlock(&philo->forks[right_fork]);
    return NULL;
}

void run_threads()
{
    t_p *philos = malloc(sizeof(t_p) * 5);
    pthread_mutex_t forks[5];
    int total = 5;

    int i = 0;
    while (i < total)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }

    i = 0;
    while (i < total)
    {
        philos[i].forks = forks;
        philos[i].n = i;
        philos[i].total = total;
        pthread_create(&philos[i].thread, NULL, live, (void *)&philos[i]);
        i++;
    }

    i = 0;
    while (i < total)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }

    i = 0;
    while (i < total)
    {
        pthread_mutex_destroy(&forks[i]);
        i++;
    }
}