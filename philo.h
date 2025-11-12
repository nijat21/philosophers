#ifndef PHILO_H
#define PHILO_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_philo
{
    int n;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_ate;
} t_philo;

int custom_atoi(char *str, int *err);
int wrong_args(int ac, char *av[]);

#endif