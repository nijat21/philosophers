#include "philo.h"

void eat(t_philo *philo)
{
	pthread_mutex_lock(philo->first_fork);
	safe_print(philo, "has taken fork");
	pthread_mutex_lock(philo->second_fork);
	safe_print(philo, "has taken fork");
	set_long(&philo->lock, &philo->born_ate_in_ms, get_ms());
	increment_long(&philo->lock, &philo->times_ate);
	set_bool(&philo->lock, &philo->is_eating, true);
	safe_print(philo, "is eating");
	safe_sleep(philo->props->t_to_eat, philo);
	set_bool(&philo->lock, &philo->is_eating, false);
	pthread_mutex_unlock(philo->first_fork);
	pthread_mutex_unlock(philo->second_fork);
}

void *track(void *arg)
{
	t_monitor *mon;
	t_props *props;

	mon = (t_monitor *)arg;
	props = mon->props;
	while (!died(props))
	{
		if (all_full(props))
			return NULL;
		usleep(10);
	}
	return (NULL);
}

bool start_times_available(t_props *props)
{
	t_philo *philo;
	int i;

	if (!get_long(&props->lock, &props->start_t_ms))
		return false;
	else
	{
		i = -1;
		while (++i < props->n_philos)
		{
			philo = &props->philos[i];
			if (!get_long(&philo->lock, &philo->born_ate_in_ms))
				return false;
		}
	}
	return true;
}

void *live(void *arg)
{
	t_philo *philo;
	t_props *props;

	philo = (t_philo *)arg;
	props = philo->props;
	increment_long(&props->lock, &props->n_ready);
	while (!start_times_available(props))
		usleep(20);

	if (philo->id % 2)
		usleep(1000);
	while (!sim_ended(props))
	{
		eat(philo);
		if (sim_ended(props))
			return NULL;
		if (get_long(&philo->lock, &philo->times_ate) == props->n_must_eat)
			return NULL;
		if (sim_ended(props))
			return NULL;
		safe_print(philo, "is sleeping");
		safe_sleep(philo->props->t_to_sleep, philo);
		if (sim_ended(props))
			return NULL;
		safe_print(philo, "is thinking");
		usleep(500);
	}
	return (NULL);
}

void *single_philo(void *arg)
{
	t_philo *philo;
	t_props *props;

	philo = (t_philo *)arg;
	props = philo->props;
	increment_long(&props->lock, &props->n_ready);
	while (!start_times_available(props))
		usleep(20);
	pthread_mutex_lock(philo->second_fork);
	safe_print(philo, "has taken fork");
	safe_sleep(philo->props->t_to_die, philo);
	pthread_mutex_unlock(philo->second_fork);
	return NULL;
}

void sim_dinner(t_props *props)
{
	t_monitor *mon;
	t_philo *philo;
	void *res;
	int i;

	res = props_init(props);
	if (!res)
		return;
	mon = malloc(sizeof(t_monitor));
	if (!mon)
		return;
	props->monitor = mon;
	mon->props = props;
	if (props->n_philos == 1)
		pthread_create(&props->philos[0].thread, NULL, single_philo, (void *)&props->philos[0]);
	else
	{
		i = -1;
		while (++i < props->n_philos)
			pthread_create(&props->philos[i].thread, NULL, live, (void *)&props->philos[i]);
	}
	while (get_long(&props->lock, &props->n_ready) != props->n_philos)
		usleep(20);
	pthread_mutex_lock(&props->lock);
	props->start_t_ms = get_ms();
	i = -1;
	while (++i < props->n_philos)
	{
		philo = &props->philos[i];
		set_long(&philo->lock, &philo->born_ate_in_ms, props->start_t_ms);
	}
	pthread_mutex_unlock(&props->lock);
	pthread_create(&mon->tracker, NULL, track, (void *)mon);
	i = -1;
	while (++i < props->n_philos)
		pthread_join(props->philos[i].thread, NULL);
	pthread_join(mon->tracker, NULL);
}
