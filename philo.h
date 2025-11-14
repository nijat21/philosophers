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
    pthread_t t;
    int n;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_eaten;
} t_philo;

typedef struct s_philos
{
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_each_philosopher_must_eat;
    t_philo *philos;
} t_philos;

int custom_atoi(char *str, int *err);
int check_and_store(int ac, char *av[], t_philos *args);

#endif