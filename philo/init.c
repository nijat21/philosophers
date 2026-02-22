/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:20:30 by nismayil          #+#    #+#             */
/*   Updated: 2026/02/21 20:32:06 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	set_start_time(t_props *props)
{
	int		i;
	t_philo	*philo;

	pthread_mutex_lock(&props->lock);
	props->start_t_ms = get_ms();
	props->start_t_set = true;
	i = -1;
	while (++i < props->n_philos)
	{
		philo = &props->philos[i];
		pthread_mutex_lock(&philo->lock);
		philo->start_t_set = true;
		philo->born_ate_in_ms = props->start_t_ms;
		pthread_mutex_unlock(&philo->lock);
	}
	pthread_mutex_unlock(&props->lock);
}

void	philos_init(t_props *props)
{
	int		i;
	t_philo	*philos;

	philos = props->philos;
	i = -1;
	while (++i < props->n_philos)
	{
		pthread_mutex_init(&philos[i].lock, NULL);
		philos[i].id = i + 1;
		philos[i].times_ate = 0;
		philos[i].start_t_set = false;
		philos[i].counted = false;
		philos[i].is_eating = false;
		philos[i].props = props;
		assign_forks(&philos[i], props->forks);
	}
}

static int	intput_to_props(int ac, char *av[], t_props *props)
{
	int	err;

	props->n_philos = custom_atoi(av[1], &err);
	props->t_to_die = custom_atoi(av[2], &err);
	props->t_to_eat = custom_atoi(av[3], &err);
	props->t_to_sleep = custom_atoi(av[4], &err);
	props->n_must_eat = -1;
	if (ac == 6)
		props->n_must_eat = custom_atoi(av[5], &err);
	props->finished_philos = 0;
	props->n_ready = 0;
	props->start_t_set = false;
	props->sim_end = false;
	props->forks = safe_malloc(sizeof(pthread_mutex_t) * props->n_philos);
	if (!props->forks)
		return (1);
	props->philos = safe_malloc(sizeof(t_philo) * props->n_philos);
	if (!props->philos)
	{
		free(props->forks);
		return (2);
	}
	return (0);
}

t_props	*props_philos_init(int ac, char **av, t_props *props)
{
	int	i;

	if (intput_to_props(ac, av, props))
	{
		free(props);
		return (NULL);
	}
	i = -1;
	while (++i < props->n_philos)
		pthread_mutex_init(&props->forks[i], NULL);
	pthread_mutex_init(&props->lock, NULL);
	pthread_mutex_init(&props->print_lock, NULL);
	philos_init(props);
	return (props);
}
