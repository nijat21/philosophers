#include "philo.h"

/*
	1.  Input to the program
	2. Proper structures
	3. Condition for a phil to die
*/
/*
	timestamp_in_ms X has taken a fork
	timestamp_in_ms X has taken a fork
	timestamp_in_ms X is eating
	timestamp_in_ms X is sleeping
	timestamp_in_ms X is thinking
	timestamp_in_ms X died
*/

static int limits_check(int res, int i)
{
	if (res < 0)
	{
		printf("Error: Arguments can't be negative\n");
		return (2);
	}
	else if (i == 1 && res > 200)
	{
		printf("Error: Number of philos shouldn't be more than 200\n");
		return (3);
	}
	else if (i > 1 && i < 5 && res < 60)
	{
		printf("Error: Time to die, time to sleep and time to eat should at least be 60ms\n");
		return (4);
	}
	else if (i != 5 && res <= 0)
	{
		printf("Error: Only [n_must_eat] can be 0\n");
		return (5);
	}
	return 0;
}

static int format_error(int ac, char **av)
{
	int i;
	int err;
	int res;
	int limit;

	err = 0;
	i = 0;
	while (++i < ac)
	{
		res = custom_atoi(av[i], &err);
		if (err)
			return (1);
		limit = limits_check(res, i);
		if (limit)
			return limit;
	}
	return 0;
}

static int invalid_input(int ac, char *av[])
{
	if (!(ac == 5 || ac == 6))
	{
		printf(RED "Error: program should be run with following arguments ->\n"
				   "./philo n_philos t_to_die t_to_eat t_to_sleep"
				   " [n_must_eat](optional)\n" RESET);
		return (1);
	}
	if (ac == 6 && atoi(av[5]) == 0)
	{
		printf(GREEN "All philosophers have eaten enough!\n" RESET);
		return (2);
	}
	if (format_error(ac, av))
		return 3;
	return 0;
}

static int intput_to_props(int ac, char *av[], t_props *args)
{
	int err;

	args->n_philos = custom_atoi(av[1], &err);
	args->t_to_die = custom_atoi(av[2], &err);
	args->t_to_eat = custom_atoi(av[3], &err);
	args->t_to_sleep = custom_atoi(av[4], &err);
	args->n_must_eat = -1;
	if (ac == 6)
		args->n_must_eat = custom_atoi(av[5], &err);
	return (0);
}

int main(int ac, char **av)
{
	int res;
	t_props *props;

	if (invalid_input(ac, av))
		return 1;
	props = safe_malloc(sizeof(t_props));
	if (!props)
		return (0);
	res = intput_to_props(ac, av, props);
	if (res)
	{
		free(props);
		return (0);
	}
	sim_dinner(props);
	clear_all(props);
}
