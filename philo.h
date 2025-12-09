/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 00:59:15 by nismayil          #+#    #+#             */
/*   Updated: 2025/12/09 15:04:42 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define RESET "\033[0m"

typedef struct s_props	t_props;

typedef enum e_time_code
{
	SECOND,
	MILLISECOND,
	MICROSECOND,
}						t_time_code;

typedef enum e_status
{
	EATING,
	SLEEPING,
	THINKING,
	DIED,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK
}						t_status;

typedef struct s_fork
{
	long				id;
	pthread_mutex_t		fork;
}						t_fork;

typedef struct s_philo
{
	long				id;
	long				born_last_ate;
	long				times_ate;
	bool				full;
	pthread_mutex_t		philo_lock;
	pthread_t			thread;
	t_fork				*first_fork;
	t_fork				*second_fork;
	t_props				*props;
}						t_philo;

typedef struct s_props
{
	long				number_of_philosophers;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				n_must_eat;
	long				start_time;
	long				n_threads_running;
	bool				all_philos_ready;
	bool				simulation_end;
	t_fork				*forks;
	t_philo				*philos;
	pthread_t			monitor;
	pthread_mutex_t		props_lock;
	pthread_mutex_t		write_lock;
}						t_props;

// utils
void					run_sim(t_props *props);
long					get_time(t_time_code time_code);
void					clean(t_props *props);
void					*track(void *arg);
void					write_status(t_status status, t_philo *philo);
bool					philo_died(t_philo *philo);
void					*track(void *arg);
int						check_and_store(int ac, char *av[], t_props *args);
void					smart_sleep(long usec, t_props *props);
void					*safe_malloc(size_t bytes);

// setters getters
void					set_bool(pthread_mutex_t *mutex, bool *dest, bool val);
bool					get_bool(pthread_mutex_t *mutex, bool *val);
void					set_long(pthread_mutex_t *mutex, long *dest, long val);
long					get_long(pthread_mutex_t *mutex, long *val);
bool					sim_ended(t_props *props);

// sync
void					wait_threads(t_props *props);
bool					all_threads_running(pthread_mutex_t *mutex,
							long *threads, long number_of_philosophers);
void					increase_long(pthread_mutex_t *mutex, long *val);
void					think(t_philo *philo, bool pre_sim);
void					sync_philos(t_philo *philo);

#endif