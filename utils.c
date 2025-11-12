#include "philo.h"

int custom_atoi(char *str, int *err)
{
    int sign;
    double res;

    sign = 1;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
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
    res *= sign;
    if (res > 2147483647 || res < -2147483648)
    {
        fprintf(stderr, "Error: Integer overflow\n");
        *err = 1;
        return 0;
    }
    return (int)res;
}

int wrong_args(int ac, char *av[])
{
    int i;
    int err;

    i = 1;
    while (i < ac)
    {
        custom_atoi(av[i], &err);
        if (err)
            return 1;
        i++;
    }
    return 0;
}
