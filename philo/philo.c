/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 21:20:58 by nismayil          #+#    #+#             */
/*   Updated: 2026/02/22 14:31:43 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	limits_check(int res, int i)
{
	if (res < 0)
	{
		ft_putstr_fd("Error: Arguments can't be negative\n", STDERR_FILENO);
		return (2);
	}
	else if (i == 1 && res > 200)
	{
		ft_putstr_fd("Error: Number of philos shouldn't be more than 200\n",
			STDERR_FILENO);
		return (3);
	}
	else if (i > 1 && i < 5 && res < 60)
	{
		ft_putstr_fd("Error: Time to die,time to sleep and time \
				to eat should at least be 60ms\n", 2);
		return (4);
	}
	else if (i != 5 && res <= 0)
	{
		ft_putstr_fd("Error: Only [n_must_eat] can be 0\n", STDERR_FILENO);
		return (5);
	}
	return (0);
}

static int	format_error(int ac, char **av)
{
	int	i;
	int	err;
	int	res;
	int	limit;

	err = 0;
	i = 0;
	while (++i < ac)
	{
		res = custom_atoi(av[i], &err);
		if (err)
			return (1);
		limit = limits_check(res, i);
		if (limit)
			return (limit);
	}
	return (0);
}

static int	invalid_input(int ac, char *av[])
{
	if (!(ac == 5 || ac == 6))
	{
		ft_putstr_fd(RED "Error: program should be run with following \
			arguments->\n ./philo n_philos t_to_die t_to_eat t_to_sleep \
			[n_must_eat](optional)\n" RESET, 2);
		return (1);
	}
	if (ac == 6 && atoi(av[5]) == 0)
	{
		ft_putstr_fd(GREEN "All philosophers have eaten enough!\n" RESET,
			STDOUT_FILENO);
		return (2);
	}
	if (format_error(ac, av))
		return (3);
	return (0);
}

int	main(int ac, char **av)
{
	t_props	*res;
	t_props	*props;

	if (invalid_input(ac, av))
		return (1);
	props = safe_malloc(sizeof(t_props));
	if (!props)
		return (2);
	res = props_philos_init(ac, av, props);
	if (!res)
		return (3);
	sim_dinner(props);
	clear_all(props);
}
