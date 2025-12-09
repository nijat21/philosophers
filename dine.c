/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dine.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 00:59:08 by nismayil          #+#    #+#             */
/*   Updated: 2025/12/09 18:12:09 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->first_fork->fork);
	write_status(TAKE_FIRST_FORK, philo);
	pthread_mutex_lock(&philo->second_fork->fork);
	write_status(TAKE_SECOND_FORK, philo);
	set_long(&philo->philo_lock, &philo->born_last_ate, get_time(MILLISECOND));
	increase_long(&philo->philo_lock, &philo->times_ate);
	write_status(EATING, philo);
	smart_sleep(philo->props->time_to_eat, philo->props);
	if (philo->props->n_must_eat > 0
		&& philo->times_ate == philo->props->n_must_eat)
		set_bool(&philo->philo_lock, &philo->full, true);
	pthread_mutex_unlock(&philo->first_fork->fork);
	pthread_mutex_unlock(&philo->second_fork->fork);
}

void	think(t_philo *philo, bool pre_sim)
{
	long	t_eat;
	long	t_sleep;
	long	t_think;

	if (!pre_sim)
		write_status(THINKING, philo);
	if (philo->props->number_of_philosophers % 2 == 0)
		return ;
	t_eat = philo->props->time_to_eat;
	t_sleep = philo->props->time_to_sleep;
	t_think = t_eat * 2 - t_sleep;
	if (t_think < 0)
		t_think = 0;
	smart_sleep(t_think * 0.42, philo->props);
}

static void	*dine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_threads(philo->props);
	set_long(&philo->philo_lock, &philo->born_last_ate, get_time(MILLISECOND));
	increase_long(&philo->props->props_lock, &philo->props->n_threads_running);
	sync_philos(philo);
	while (!sim_ended(philo->props))
	{
		if (get_bool(&philo->philo_lock, &philo->full))
			break ;
		eat(philo);
		write_status(SLEEPING, philo);
		smart_sleep(philo->props->time_to_sleep, philo->props);
		think(philo, false);
	}
	return (NULL);
}

static void	*lone_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_threads(philo->props);
	set_long(&philo->philo_lock, &philo->born_last_ate, get_time(MILLISECOND));
	increase_long(&philo->props->props_lock, &philo->props->n_threads_running);
	write_status(TAKE_FIRST_FORK, philo);
	while (!sim_ended(philo->props))
		usleep(200);
	return (NULL);
}

void	run_sim(t_props *props)
{
	int	i;

	i = -1;
	if (props->number_of_philosophers == 1)
		pthread_create(&props->philos[0].thread, NULL, lone_philo,
			(void *)&props->philos[0]);
	else
		while (++i < props->number_of_philosophers)
			pthread_create(&props->philos[i].thread, NULL, dine,
				(void *)&props->philos[i]);
	pthread_create(&props->monitor, NULL, track, (void *)props);
	set_long(&props->props_lock, &props->start_time, get_time(MILLISECOND));
	set_bool(&props->props_lock, &props->all_philos_ready, true);
	i = -1;
	while (++i < props->number_of_philosophers)
		pthread_join(props->philos[i].thread, NULL);
	set_bool(&props->props_lock, &props->simulation_end, true);
	pthread_join(props->monitor, NULL);
}
