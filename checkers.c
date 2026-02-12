#include "philo.h"

bool sim_ended(t_props *props)
{
	bool res;

	res = false;
	pthread_mutex_lock(&props->lock);
	if (props->some_p_died || props->sim_end)
		res = true;
	pthread_mutex_unlock(&props->lock);
	return res;
}

void wait_threads(t_props *props)
{
	long ready;

	ready = get_long(&props->lock, &props->n_ready);
	while (ready != props->n_philos)
	{
		usleep(10);
		ready = get_long(&props->lock, &props->n_ready);
	}
}

bool all_full(t_props *props)
{
	int i;
	t_philo *philo;

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
				return true;
			}
			pthread_mutex_unlock(&props->lock);
		}
		else
			pthread_mutex_unlock(&philo->lock);
	}
	return false;
}

bool died(t_props *props)
{
	long last_ate;
	bool is_eating;
	int i;
	t_philo *philo;

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
