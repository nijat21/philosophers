/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setters_getters.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 00:59:25 by nismayil          #+#    #+#             */
/*   Updated: 2025/12/09 00:59:27 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void set_bool(pthread_mutex_t *mutex, bool *dest, bool val)
{
    pthread_mutex_lock(mutex);
    *dest = val;
    pthread_mutex_unlock(mutex);
}

bool get_bool(pthread_mutex_t *mutex, bool *val)
{
    bool res;

    pthread_mutex_lock(mutex);
    res = *val;
    pthread_mutex_unlock(mutex);
    return res;
}

void set_long(pthread_mutex_t *mutex, long *dest, long val)
{
    pthread_mutex_lock(mutex);
    *dest = val;
    pthread_mutex_unlock(mutex);
}

long get_long(pthread_mutex_t *mutex, long *val)
{
    long res;

    pthread_mutex_lock(mutex);
    res = *val;
    pthread_mutex_unlock(mutex);
    return res;
}

bool sim_ended(t_props *props)
{
    return get_bool(&props->props_lock, &props->simulation_end);
}
