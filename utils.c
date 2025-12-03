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
    if (ac == 6)
        args->number_of_times_each_philosopher_must_eat = custom_atoi(av[5], &err);
    return 0;
}
