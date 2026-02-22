/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:20:52 by nismayil          #+#    #+#             */
/*   Updated: 2026/02/22 14:51:22 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->first_fork);
	safe_print(philo, "has taken fork");
	pthread_mutex_lock(philo->second_fork);
	safe_print(philo, "has taken fork");
	set_long(&philo->lock, &philo->born_ate_in_ms, get_ms());
	increment_long(&philo->lock, &philo->times_ate);
	set_bool(&philo->lock, &philo->is_eating, true);
	safe_print(philo, "is eating");
	safe_sleep(philo->props->t_to_eat, philo);
	set_bool(&philo->lock, &philo->is_eating, false);
	pthread_mutex_unlock(philo->first_fork);
	pthread_mutex_unlock(philo->second_fork);
}

void	*track(void *arg)
{
	t_monitor	*mon;
	t_props		*props;
	t_philo		*philo;
	int			i;

	mon = (t_monitor *)arg;
	props = mon->props;
	while (1)
	{
		i = -1;
		while (++i < props->n_philos)
		{
			philo = &props->philos[i];
			pthread_mutex_lock(&philo->lock);
			if (died(props, philo) || all_full(props, philo))
			{
				pthread_mutex_unlock(&philo->lock);
				return (NULL);
			}
			pthread_mutex_unlock(&philo->lock);
		}
		usleep(100);
	}
	return (NULL);
}

void	*live(void *arg)
{
	t_philo	*philo;
	t_props	*props;

	philo = (t_philo *)arg;
	props = philo->props;
	increment_long(&philo->props->lock, &philo->props->n_ready);
	while (!start_times_available(philo->props))
		;
	if (philo->id % 2)
		usleep(1000);
	while (!sim_ended(props))
	{
		eat(philo);
		if (sim_ended(props))
			return (NULL);
		if (get_long(&philo->lock, &philo->times_ate) == props->n_must_eat)
			return (NULL);
		safe_print(philo, "is sleeping");
		safe_sleep(philo->props->t_to_sleep, philo);
		if (sim_ended(props))
			return (NULL);
		safe_print(philo, "is thinking");
		usleep(300);
	}
	return (NULL);
}

void	*single_philo(void *arg)
{
	t_philo	*philo;
	t_props	*props;

	philo = (t_philo *)arg;
	props = philo->props;
	increment_long(&props->lock, &props->n_ready);
	while (!start_times_available(props))
		;
	pthread_mutex_lock(philo->second_fork);
	safe_print(philo, "has taken fork");
	safe_sleep(philo->props->t_to_die, philo);
	pthread_mutex_unlock(philo->second_fork);
	return (NULL);
}

void	sim_dinner(t_props *props)
{
	t_monitor	*mon;
	int			i;

	mon = malloc(sizeof(t_monitor));
	if (!mon)
		return ;
	props->monitor = mon;
	mon->props = props;
	if (props->n_philos == 1)
		pthread_create(&props->philos[0].thread, NULL,
			single_philo, (void *)&props->philos[0]);
	else
	{
		i = -1;
		while (++i < props->n_philos)
			pthread_create(&props->philos[i].thread, NULL, live,
				(void *)&props->philos[i]);
	}
	while (get_long(&props->lock, &props->n_ready) != props->n_philos)
		;
	set_start_time(props);
	pthread_create(&mon->tracker, NULL, track, (void *)mon);
	join_threads(props);
}
