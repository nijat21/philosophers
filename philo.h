#ifndef PHILO_H
#define PHILO_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

typedef struct s_props
{
    int number_of_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int number_of_times_each_philosopher_must_eat;
    int some_philo_died;
    int finished_philos;
    int simulation_end;
    long start_time;
    pthread_mutex_t death_lock;
    pthread_mutex_t print_lock;
} t_props;

typedef enum e_state
{
    EATING,
    SLEEPING,
    THINKING,
} t_state;

typedef struct s_philo
{
    int id;
    pthread_t thread;
    pthread_mutex_t *forks;
    pthread_mutex_t state_lock;
    int number_of_times_eaten;
    long born_or_last_ate_in_ms;
    t_props *props;
    t_state state;
} t_philo;

typedef struct s_monitor
{
    pthread_t tracker;
    t_props *props;
    t_philo *philos;
} t_monitor;

int custom_atoi(char *str, int *err);
int check_and_store(int ac, char *av[], t_props *args);

void run_threads(t_props *props);

int max(int a, int b);
int min(int a, int b);
long get_ms(void);
int died_or_ended(t_props *props);
void smart_sleep(long ms, t_philo *philo);
void safe_print(t_philo *philo, char *msg);
int sim_step_or_end(t_philo *philo);
void philo_sleep(t_philo *philo);
void philo_think(t_philo *philo);
void assign_forks(t_philo *philo, int *first_fork, int *second_fork);

#endif