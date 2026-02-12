#include "philo.h"

int custom_atoi(char *str, int *err)
{
	double res;

	while (*str == ' ')
		str++;
	if (*str == '+')
		str++;
	res = 0;
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
		{
			printf("Error: Non-numeric character\n");
			*err = 1;
			return (0);
		}
		res = res * 10 + (*str - '0');
		str++;
	}
	if (res > 2147483647)
	{
		printf("Error: Integer overflow\n");
		*err = 1;
		return (0);
	}
	return ((int)res);
}

long get_ms(void)
{
	struct timeval tv;

	if (gettimeofday(&tv, NULL) < 0)
	{
		perror("gettimeofday");
		return (-1);
	}
	return (long)(tv.tv_sec * 1e3 + tv.tv_usec / 1e3);
}

void assign_forks(t_philo *philo, pthread_mutex_t *forks)
{
	int n_philos;
	long first;
	long second;

	n_philos = philo->props->n_philos;
	if (philo->id % 2)
	{
		first = philo->id % n_philos;
		second = philo->id - 1;
	}
	else
	{
		first = philo->id - 1;
		second = philo->id % n_philos;
	}
	philo->first_fork = &forks[first];
	philo->second_fork = &forks[second];
}

void clear_all(t_props *props)
{
	int i;

	i = -1;
	while (++i < props->n_philos)
		pthread_mutex_destroy(&props->forks[i]);
	pthread_mutex_destroy(&props->lock);
	pthread_mutex_destroy(&props->print_lock);
	i = -1;
	while (++i < props->n_philos)
		pthread_mutex_destroy(&props->philos[i].lock);
	free(props->forks);
	free(props->philos);
	free(props->monitor);
	free(props);
}