/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/27 21:04:41 by nismayil          #+#    #+#             */
/*   Updated: 2026/02/28 19:10:35 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*single_philo(void *arg)
{
	t_philo	*philo;
	t_props	*props;

	philo = (t_philo *)arg;
	props = philo->props;
	increment_long(&props->lock, &props->n_ready);
	while (!start_times_available(props))
		usleep(50);
	pthread_mutex_lock(philo->second_fork);
	safe_print(philo, "has taken fork");
	safe_sleep(philo->props->t_to_die, philo);
	pthread_mutex_unlock(philo->second_fork);
	return (NULL);
}
