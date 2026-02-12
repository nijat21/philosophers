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
	bool some_p_died;
	long rem;
	long elaps;

	start = get_ms();
	elaps = 0;
	while (elaps < ms)
	{
		rem = ms - elaps;
		some_p_died = get_bool(&philo->props->lock, &philo->props->some_p_died);
		if (some_p_died)
			return;
		if (rem > 2)
			usleep(1000);
		else if (rem > 1)
			usleep(500);
		else
			usleep(300);
		elaps = get_ms() - start;
	}
}

void safe_print(t_philo *philo, char *msg)
{
	t_props *props;
	bool some_p_died;
	bool sim_end;

	props = philo->props;
	some_p_died = get_bool(&props->lock, &props->some_p_died);
	sim_end = get_bool(&props->lock, &props->sim_end);
	if (strcmp(msg, "died") == 0 && !sim_end)
	{
		pthread_mutex_lock(&props->print_lock);
		printf(RED "%ld" RESET " %ld %s\n", get_ms() - props->start_t_ms, philo->id,
			   msg);
		pthread_mutex_unlock(&props->print_lock);
		if (!some_p_died)
			some_p_died = true;
		return;
	}
	if (!some_p_died && !sim_end)
	{
		pthread_mutex_lock(&props->print_lock);
		printf(GREEN "%ld" RESET " %ld %s\n", get_ms() - props->start_t_ms,
			   philo->id, msg);
		pthread_mutex_unlock(&props->print_lock);
	}
}
