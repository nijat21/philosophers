#include "philo.h"

int sim_step_or_end(t_philo *philo)
{
    int eaten;

    pthread_mutex_lock(&philo->state_lock);
    eaten = philo->times_ate;
    pthread_mutex_unlock(&philo->state_lock);
    if (philo->props->number_of_times_each_philosopher_must_eat != -1 &&
        eaten == philo->props->number_of_times_each_philosopher_must_eat)
    {
        pthread_mutex_lock(&philo->props->death_lock);
        philo->props->finished_philos += 1;
        if (philo->props->finished_philos == philo->props->number_of_philosophers)
            philo->props->simulation_end = true;
        pthread_mutex_unlock(&philo->props->death_lock);
        return 1;
    }
    return 0;
}

void philo_sleep(t_philo *philo)
{
    pthread_mutex_lock(&philo->state_lock);
    philo->is_eating = false;
    pthread_mutex_unlock(&philo->state_lock);
    safe_print(philo, "is sleeping");
    smart_sleep(philo->props->time_to_sleep, philo);
}

long get_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}