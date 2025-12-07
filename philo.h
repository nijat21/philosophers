#ifndef PHILO_H
#define PHILO_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <errno.h>
#include <stdbool.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

typedef struct s_props t_props;

typedef struct s_fork
{
    int id;
    pthread_mutex_t fork;
} t_fork;

typedef struct s_philo
{
    int id;
    int times_ate;
    long born_or_last_ate_in_ms;
    bool is_eating;
    pthread_mutex_t state_lock;
    pthread_t thread;
    t_fork *first_fork;
    t_fork *second_fork;
    t_props *props;
} t_philo;

typedef struct s_props
{
    int number_of_philosophers;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    int number_of_times_each_philosopher_must_eat;
    int finished_philos;
    long start_time;
    bool all_philos_ready;
    bool simulation_end;
    bool some_philo_died;
    t_fork *forks;
    t_philo *philos;
    pthread_mutex_t death_lock;
    pthread_mutex_t print_lock;
} t_props;

typedef struct s_monitor
{
    pthread_t tracker;
    t_props *props;
    t_philo *philos;
} t_monitor;

void run_threads(t_props *props);

int max(int a, int b);
int min(int a, int b);
long get_ms(void);
int died_or_ended(t_props *props);
int sim_step_or_end(t_philo *philo);
void philo_sleep(t_philo *philo);
void assign_forks(t_philo *philo, t_fork *forks, int ph_position);
// parsers
int custom_atoi(char *str, int *err);
int check_and_store(int ac, char *av[], t_props *args);

// safe functions
void safe_print(t_philo *philo, char *msg);
void smart_sleep(long ms, t_philo *philo);
void *safe_malloc(size_t bytes);

#endif