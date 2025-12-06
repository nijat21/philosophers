#include "philo.h"

int sim_step_or_end(t_philo *philo)
{
    pthread_mutex_lock(&philo->state_lock);
    if (philo->props->number_of_times_each_philosopher_must_eat != -1 &&
        philo->number_of_times_eaten == philo->props->number_of_times_each_philosopher_must_eat)
    {
        pthread_mutex_lock(&philo->props->death_lock);
        philo->props->finished_philos += 1;
        if (philo->props->finished_philos == philo->props->number_of_philosophers)
            philo->props->simulation_end = 1;
        pthread_mutex_unlock(&philo->props->death_lock);
        pthread_mutex_unlock(&philo->state_lock);
        return 1;
    }
    pthread_mutex_unlock(&philo->state_lock);
    return 0;
}

void philo_sleep(t_philo *philo)
{
    pthread_mutex_lock(&philo->state_lock);
    philo->state = SLEEPING;
    pthread_mutex_unlock(&philo->state_lock);
    safe_print(philo, "is sleeping");
    smart_sleep(philo->props->time_to_sleep, philo);
}

void philo_think(t_philo *philo)
{
    pthread_mutex_lock(&philo->state_lock);
    philo->state = THINKING;
    pthread_mutex_unlock(&philo->state_lock);
    safe_print(philo, "is thinking");
}

void assign_forks(t_philo *philo, int *first_fork, int *second_fork)
{
    if (philo->id % 2 == 0)
    {
        *first_fork = (philo->id + 1) % philo->props->number_of_philosophers;
        *second_fork = philo->id;
    }
    else
    {
        *first_fork = philo->id;
        *second_fork = (philo->id + 1) % philo->props->number_of_philosophers;
    }
}

long get_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}