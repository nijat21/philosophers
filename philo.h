#ifndef PHILO_H
#define PHILO_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_props
{
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_each_philosopher_must_eat;
    int some_philo_died;
    pthread_mutex_t death_lock;
    pthread_mutex_t state_update_lock;
    int finished_philos;
    int simulation_end;
} t_props;

typedef struct s_philo
{
    int id;
    pthread_t thread;
    pthread_mutex_t *forks;
    int forks_picked;
    int ate_this_round;
    int number_of_times_eaten;
    long born_or_last_ate_in_ms;
    long start;
    t_props *props;
} t_philo;

int custom_atoi(char *str, int *err);
int check_and_store(int ac, char *av[], t_props *args);

void run_threads(t_props *props);

#endif