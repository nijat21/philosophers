/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/22 14:32:44 by nismayil          #+#    #+#             */
/*   Updated: 2026/02/27 21:01:10 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <errno.h>
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

typedef struct s_props		t_props;
typedef struct s_monitor	t_monitor;

typedef struct s_philo
{
	long					id;
	long					times_ate;
	long					born_ate_in_ms;
	bool					start_t_set;
	bool					counted;
	bool					is_eating;
	pthread_mutex_t			lock;
	pthread_t				thread;
	pthread_mutex_t			*first_fork;
	pthread_mutex_t			*second_fork;
	t_props					*props;
}							t_philo;

typedef struct s_props
{
	long					n_philos;
	long					t_to_die;
	long					t_to_eat;
	long					t_to_sleep;
	long					n_must_eat;
	long					finished_philos;
	long					n_ready;
	long					start_t_ms;
	bool					start_t_set;
	bool					sim_end;
	pthread_mutex_t			*forks;
	t_philo					*philos;
	t_monitor				*monitor;
	pthread_mutex_t			lock;
	pthread_mutex_t			print_lock;
}							t_props;

typedef struct s_monitor
{
	pthread_t				tracker;
	t_props					*props;
}							t_monitor;

// simulation
void						*single_philo(void *arg);
void						sim_dinner(t_props *props);

// checkers
bool						died(t_props *props, t_philo *philo);
bool						all_full(t_props *props, t_philo *philo);
bool						sim_ended(t_props *props);
bool						start_times_available(t_props *props);

// init
void						set_start_time(t_props *props);
void						philos_init(t_props *props);
t_props						*props_philos_init(int ac, char **av,
								t_props *props);

// setters getters
bool						get_bool(pthread_mutex_t *lock, bool *val);
void						set_bool(pthread_mutex_t *lock, bool *val,
								bool new_val);
long						get_long(pthread_mutex_t *lock, long *val);
void						set_long(pthread_mutex_t *lock, long *val,
								long new_val);
void						increment_long(pthread_mutex_t *lock, long *val);

// safe functions
void						safe_print(t_philo *philo, char *msg);
void						safe_sleep(long ms, t_philo *philo);
void						*safe_malloc(size_t bytes);

// utils
int							custom_atoi(char *str, int *err);
void						clear_all(t_props *props);
long						get_ms(void);
void						assign_forks(t_philo *philo,
								pthread_mutex_t *forks);
void						ft_putstr_fd(char *s, int fd);
void						join_threads(t_props *props);

#endif
