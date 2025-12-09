/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sync_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 00:59:11 by nismayil          #+#    #+#             */
/*   Updated: 2025/12/09 15:11:40 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_threads(t_props *props)
{
	while (!get_bool(&props->props_lock, &props->all_philos_ready))
		;
}

bool	all_threads_running(pthread_mutex_t *mutex, long *n_of_threads,
		long number_of_philosophers)
{
	bool	res;

	res = false;
	pthread_mutex_lock(mutex);
	if (*n_of_threads == number_of_philosophers)
		res = true;
	pthread_mutex_unlock(mutex);
	return (res);
}

void	increase_long(pthread_mutex_t *mutex, long *val)
{
	pthread_mutex_lock(mutex);
	(*val)++;
	pthread_mutex_unlock(mutex);
}

void	sync_philos(t_philo *philo)
{
	if (philo->props->number_of_philosophers % 2 == 0)
	{
		if (philo->id % 2 == 0)
			smart_sleep(3e4, philo->props);
	}
	else
	{
		if (philo->id % 2)
			think(philo, true);
	}
}
