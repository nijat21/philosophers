/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 00:59:22 by nismayil          #+#    #+#             */
/*   Updated: 2025/12/09 15:11:23 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void write_status(t_status status, t_philo *philo)
{
    long elapsed;

    if (philo->full)
        return;
    elapsed = get_time(MILLISECOND) - philo->props->start_time;
    pthread_mutex_lock(&philo->props->write_lock);
    if ((status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK) && !sim_ended(philo->props))
        printf(GREEN "%ld" RESET " %ld has taken a fork\n", elapsed, philo->id);
    else if (status == EATING && !sim_ended(philo->props))
        printf(GREEN "%ld" RESET " %ld is eating\n", elapsed, philo->id);
    else if (status == SLEEPING && !sim_ended(philo->props))
        printf(GREEN "%ld" RESET " %ld is sleeping\n", elapsed, philo->id);
    else if (status == THINKING && !sim_ended(philo->props))
        printf(GREEN "%ld" RESET " %ld is thinking\n", elapsed, philo->id);
    else if (status == DIED)
        printf(RED "%ld" RESET " %ld died\n", elapsed, philo->id);
    pthread_mutex_unlock(&philo->props->write_lock);
}
