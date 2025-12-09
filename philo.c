/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:08:47 by nismayil          #+#    #+#             */
/*   Updated: 2025/12/09 18:14:57 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *philo, t_fork *forks, int ph_position)
{
	int	ph_number;

	ph_number = philo->props->number_of_philosophers;
	philo->first_fork = &forks[(ph_position + 1) % ph_number];
	philo->second_fork = &forks[ph_position];
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[ph_position];
		philo->second_fork = &forks[(ph_position + 1) % ph_number];
	}
}

static void	philos_init(t_props *props)
{
	int		i;
	t_philo	*philos;

	philos = props->philos;
	i = -1;
	while (++i < props->number_of_philosophers)
	{
		philos[i].id = i + 1;
		philos[i].times_ate = 0;
		philos[i].full = false;
		philos[i].props = props;
		pthread_mutex_init(&philos[i].philo_lock, NULL);
		assign_forks(&philos[i], props->forks, i);
	}
}

static void	*props_init(t_props *props)
{
	int	i;

	props->n_threads_running = 0;
	props->all_philos_ready = false;
	props->simulation_end = false;
	props->forks = safe_malloc(sizeof(t_fork) * props->number_of_philosophers);
	if (!props->forks)
		return (NULL);
	props->philos = safe_malloc(sizeof(t_philo)
			* props->number_of_philosophers);
	if (!props->philos)
		return (NULL);
	pthread_mutex_init(&props->props_lock, NULL);
	pthread_mutex_init(&props->write_lock, NULL);
	i = -1;
	while (++i < props->number_of_philosophers)
	{
		pthread_mutex_init(&props->forks[i].fork, NULL);
		props->forks[i].id = i;
	}
	philos_init(props);
	return (props);
}

static bool	validate_input(int ac, char **av)
{
	if (!(ac == 5 || ac == 6))
	{
		printf(RED "Error: program should be run with following arguments ->\n"
			"./philo number_of_philosophers time_to_die time_to_eat "
			"time_to_sleep [n_must_eat](optional)\n" RESET);
		return (false);
	}
	if (ac == 6 && atoi(av[5]) == 0)
		return (false);
	return (true);
}

int	main(int ac, char **av)
{
	int		parse;
	void	*res;
	t_props	*props;

	if (!validate_input(ac, av))
		return (0);
	props = safe_malloc(sizeof(t_props));
	if (!props)
		return (0);
	parse = check_and_store(ac, av, props);
	if (parse)
	{
		free(props);
		return (0);
	}
	res = props_init(props);
	if (res == NULL)
	{
		clean(props);
		return (0);
	}
	run_sim(props);
	clean(props);
}
