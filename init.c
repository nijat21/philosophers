#include "philo.h"

void philos_init(t_props *props)
{
	int i;
	t_philo *philos;

	philos = props->philos;
	i = -1;
	while (++i < props->n_philos)
	{
		pthread_mutex_init(&philos[i].lock, NULL);
		philos[i].id = i + 1;
		philos[i].times_ate = 0;
		philos[i].counted = false;
		philos[i].is_eating = false;
		philos[i].props = props;
		assign_forks(&philos[i], props->forks);
	}
}

void *props_init(t_props *props)
{
	int i;

	props->finished_philos = 0;
	props->some_p_died = false;
	props->sim_end = false;
	props->forks = safe_malloc(sizeof(pthread_mutex_t) * props->n_philos);
	if (!props->forks)
		return (NULL);
	props->philos = safe_malloc(sizeof(t_philo) * props->n_philos);
	if (!props->philos)
		return (NULL);
	i = -1;
	while (++i < props->n_philos)
		pthread_mutex_init(&props->forks[i], NULL);
	pthread_mutex_init(&props->lock, NULL);
	pthread_mutex_init(&props->print_lock, NULL);
	philos_init(props);
	return (props);
}
