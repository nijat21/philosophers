/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:20:48 by nismayil          #+#    #+#             */
/*   Updated: 2026/02/14 00:00:39 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	sim_ended(t_props *props)
{
	bool	res;

	res = false;
	pthread_mutex_lock(&props->lock);
	if (props->some_p_died || props->sim_end)
		res = true;
	pthread_mutex_unlock(&props->lock);
	return (res);
}

bool	all_full(t_props *props)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < props->n_philos && (props->n_must_eat != -1))
	{
		philo = &props->philos[i];
		pthread_mutex_lock(&philo->lock);
		if (!philo->counted && philo->times_ate == props->n_must_eat)
		{
			philo->counted = true;
			pthread_mutex_unlock(&philo->lock);
			pthread_mutex_lock(&props->lock);
			props->finished_philos++;
			if (props->finished_philos == props->n_philos)
			{
				props->sim_end = true;
				pthread_mutex_unlock(&props->lock);
				return (true);
			}
			pthread_mutex_unlock(&props->lock);
		}
		else
			pthread_mutex_unlock(&philo->lock);
	}
	return (false);
}

bool	died(t_props *props)
{
	long	last_ate;
	bool	is_eating;
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < props->n_philos)
	{
		philo = &props->philos[i];
		pthread_mutex_lock(&philo->lock);
		last_ate = philo->born_ate_in_ms;
		is_eating = philo->is_eating;
		pthread_mutex_unlock(&philo->lock);
		if (!is_eating && ((get_ms() - last_ate) > props->t_to_die))
		{
			set_bool(&philo->props->lock, &philo->props->some_p_died, true);
			usleep(10);
			safe_print(philo, "died");
			return (true);
		}
	}
	return (false);
}

bool	start_times_available(t_props *props)
{
	t_philo *philo;
	int i;

	if (!get_bool(&props->lock, &props->start_t_set))
		return (false);
	else
	{
		i = -1;
		while (++i < props->n_philos)
		{
			philo = &props->philos[i];
			if (!get_bool(&philo->lock, &philo->start_t_set))
				return (false);
		}
	}
	return (true);
}