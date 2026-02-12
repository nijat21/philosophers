#include "philo.h"

void wait_threads(t_props *props)
{
    long ready;

    ready = get_long(&props->lock, &props->n_ready);
    while (ready != props->n_philos)
    {
        usleep(100);
        ready = get_long(&props->lock, &props->n_ready);
    }
}