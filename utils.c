/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 00:59:01 by nismayil          #+#    #+#             */
/*   Updated: 2025/12/09 11:19:38 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_time(t_time_code time_code)
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL))
    {
        printf("Error: Couldn't get time\n");
        return -1;
    }
    if (time_code == SECOND)
        return (tv.tv_sec + tv.tv_usec / 1e6);
    else if (time_code == MILLISECOND)
        return (tv.tv_sec * 1e3 + tv.tv_usec / 1e3);
    else if (time_code == MICROSECOND)
        return tv.tv_sec * 1e6 + tv.tv_usec;
    else
    {
        printf("Error: Wrong time code\n");
        return -1;
    }
}

void smart_sleep(long usec, t_props *props)
{
    long start;
    long elapsed;
    long rem;

    start = get_time(MICROSECOND);
    while (get_time(MICROSECOND) - start < usec)
    {
        if (sim_ended(props))
            break;
        elapsed = get_time(MICROSECOND) - start;
        rem = usec - elapsed;
        if (rem > 1e3)
            usleep(rem / 2);
        else
        {
            while (get_time(MICROSECOND) - start < usec)
                ;
        }
    }
}

void clean(t_props *props)
{
    int i;
    t_philo *philo;

    i = -1;
    while (++i < props->number_of_philosophers)
    {
        philo = &props->philos[i];
        pthread_mutex_destroy(&philo->philo_lock);
        pthread_mutex_destroy(&props->forks[i].fork);
    }
    pthread_mutex_destroy(&props->props_lock);
    pthread_mutex_destroy(&props->write_lock);
    free(props->forks);
    free(props->philos);
    free(props);
}

void *safe_malloc(size_t bytes)
{
    void *mem;

    mem = malloc(bytes);
    if (!mem)
    {
        printf("Error: Couldn't allocate memory\n");
        return NULL;
    }
    return mem;
}
