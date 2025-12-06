#include "philo.h"

int custom_atoi(char *str, int *err)
{
    double res;

    if (*str == '+')
        str++;
    res = 0;
    while (*str)
    {
        if (!(*str >= '0' && *str <= '9'))
        {
            fprintf(stderr, "Error: Non-numeric character\n");
            *err = 1;
            return 0;
        }
        res = res * 10 + (*str - '0');
        str++;
    }
    if (res > 2147483647 || res < -2147483648)
    {
        fprintf(stderr, "Error: Integer overflow\n");
        *err = 1;
        return 0;
    }
    return (int)res;
}

int check_and_store(int ac, char *av[], t_props *args)
{
    int i;
    int err;
    int res;

    err = 0;
    i = 1;
    while (i < ac)
    {
        res = custom_atoi(av[i], &err);
        if (err)
            return 1;
        else if (res < 0)
        {
            printf("Error: Arguments can't be negative\n");
            return 1;
        }
        else if (i != 5 && res <= 0)
        {
            printf("Error: Only [number_of_times_each_philosopher_must_eat] can be 0\n");
            return 1;
        }
        i++;
    }
    args->number_of_philosophers = custom_atoi(av[1], &err);
    args->time_to_die = custom_atoi(av[2], &err);
    args->time_to_eat = custom_atoi(av[3], &err);
    args->time_to_sleep = custom_atoi(av[4], &err);
    args->number_of_times_each_philosopher_must_eat = -1;
    if (ac == 6)
        args->number_of_times_each_philosopher_must_eat = custom_atoi(av[5], &err);
    return 0;
}

int died_or_ended(t_props *props)
{
    int died_end;

    died_end = 0;
    pthread_mutex_lock(&props->death_lock);
    died_end = props->some_philo_died || props->simulation_end;
    pthread_mutex_unlock(&props->death_lock);
    return died_end;
}

void smart_sleep(long ms, t_philo *philo)
{
    long start;
    int philo_died;

    start = get_ms();
    while (get_ms() - start < ms)
    {
        pthread_mutex_lock(&philo->props->death_lock);
        philo_died = philo->props->some_philo_died;
        pthread_mutex_unlock(&philo->props->death_lock);
        if (philo_died)
            break;
        usleep(100);
    }
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