/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:20:48 by nismayil          #+#    #+#             */
/*   Updated: 2026/02/22 02:28:52 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool sim_ended(t_props *props)
{
	return (get_bool(&props->lock, &props->sim_end));
}

bool all_full(t_props *props, t_philo *philo)
{
	if (!philo->counted && philo->times_ate == props->n_must_eat)
	{
		philo->counted = true;
		pthread_mutex_lock(&props->lock);
		props->finished_philos++;
		if (props->finished_philos == props->n_philos)
		{
			props->sim_end = true;
			usleep(10);
			pthread_mutex_unlock(&props->lock);
			return (true);
		}
		pthread_mutex_unlock(&props->lock);
	}
	return (false);
}

/*
	Print after death:
	philo n -has been sleeping and after some philo died at the same time he's now thinking and that's printed
	400 19 is sleeping
	601 2 died
	603 19 is thinking
*/

bool died(t_props *props, t_philo *philo)
{
	long last_ate;
	bool is_eating;

	last_ate = philo->born_ate_in_ms;
	is_eating = philo->is_eating;
	if (!is_eating && ((get_ms() - last_ate) >= props->t_to_die))
	{
		set_bool(&philo->props->lock, &philo->props->sim_end, true);
		pthread_mutex_lock(&props->print_lock);
		printf(RED "%ld" RESET " %ld %s\n", get_ms() - props->start_t_ms,
			   philo->id, "died");
		usleep(10);
		pthread_mutex_unlock(&props->print_lock);
		return (true);
	}

	return (false);
}

bool start_times_available(t_props *props)
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
