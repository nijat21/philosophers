#include "philo.h"

void *safe_malloc(size_t bytes)
{
    void *mem;

    mem = malloc(bytes);
    if (!mem)
    {
        printf("Error: Couldn't allocate memory\n");
        return NULL;
    }
    return mem;
}

void safe_print(t_philo *philo, char *msg)
{
    int philo_died;

    if (strcmp(msg, "died") == 0)
    {
        pthread_mutex_lock(&philo->props->print_lock);
        printf(RED "%ld" RESET " %d %s\n", get_ms() - philo->props->start_time, philo->id + 1, msg);
        pthread_mutex_unlock(&philo->props->print_lock);
        return;
    }
    pthread_mutex_lock(&philo->props->death_lock);
    philo_died = philo->props->some_philo_died;
    pthread_mutex_unlock(&philo->props->death_lock);
    if (!philo_died)
    {
        pthread_mutex_lock(&philo->props->print_lock);
        printf(GREEN "%ld" RESET " %d %s\n", get_ms() - philo->props->start_time, philo->id + 1, msg);
        pthread_mutex_unlock(&philo->props->print_lock);
    }
}