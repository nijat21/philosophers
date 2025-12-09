/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tracker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 00:59:18 by nismayil          #+#    #+#             */
/*   Updated: 2025/12/09 13:30:53 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool philo_died(t_philo *philo)
{
    long elapsed;
    long time_to_die;

    elapsed = get_time(MILLISECOND) - get_long(&philo->philo_lock, &philo->born_last_ate);
    time_to_die = philo->props->time_to_die / 1e3;

    if (get_bool(&philo->philo_lock, &philo->full))
        return false;
    if (elapsed > time_to_die)
        return true;
    else
        return false;
}

void *track(void *arg)
{
    int i;
    t_props *props;

    props = (t_props *)arg;
    while (!all_threads_running(&props->props_lock, &props->n_threads_running, props->number_of_philosophers))
        ;
    while (!sim_ended(props))
    {
        i = -1;
        while (++i < props->number_of_philosophers && !sim_ended(props))
        {
            if (philo_died(&props->philos[i]))
            {
                set_bool(&props->props_lock, &props->simulation_end, true);
                write_status(DIED, &props->philos[i]);
                return NULL;
            }
        }
        usleep(100);
    }
    return NULL;
}
