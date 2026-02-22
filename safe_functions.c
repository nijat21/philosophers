/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safe_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:20:36 by nismayil          #+#    #+#             */
/*   Updated: 2026/02/22 02:34:11 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *safe_malloc(size_t bytes)
{
	void *mem;

	mem = malloc(bytes);
	if (!mem)
	{
		printf("Error: Couldn't allocate memory\n");
		return (NULL);
	}
	return (mem);
}

void safe_sleep(long ms, t_philo *philo)
{
	long start;

	start = get_ms();
	while (get_ms() - start < ms)
	{
		if (get_bool(&philo->props->lock, &philo->props->sim_end))
			return;
		usleep(100);
	}
}

void safe_print(t_philo *philo, char *msg)
{
	t_props *props;

	props = philo->props;
	pthread_mutex_lock(&props->lock);
	if (!props->sim_end)
	{
		pthread_mutex_lock(&props->print_lock);
		printf(GREEN "%ld" RESET " %ld %s\n", get_ms() - props->start_t_ms,
			   philo->id, msg);
		pthread_mutex_unlock(&props->print_lock);
	}
	pthread_mutex_unlock(&props->lock);
}
