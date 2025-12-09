/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nismayil <nismayil@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 18:09:55 by nismayil          #+#    #+#             */
/*   Updated: 2025/12/09 18:09:57 by nismayil         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	custom_atoi(char *str, int *err)
{
	double	res;

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
	if (res > INT_MAX)
	{
		printf("Error: Integer overflow\n");
		*err = 1;
		return (0);
	}
	return ((int)res);
}

static void	asign_args(int ac, char *av[], t_props *args, int *err)
{
	args->number_of_philosophers = custom_atoi(av[1], err);
	args->time_to_die = custom_atoi(av[2], err) * 1e3;
	args->time_to_eat = custom_atoi(av[3], err) * 1e3;
	args->time_to_sleep = custom_atoi(av[4], err) * 1e3;
	args->n_must_eat = -1;
	if (ac == 6)
		args->n_must_eat = custom_atoi(av[5], err);
}

int	check_and_store(int ac, char *av[], t_props *args)
{
	int	i;
	int	err;
	int	res;

	err = 0;
	i = 1;
	while (i < ac)
	{
		res = custom_atoi(av[i], &err);
		if (err)
			return (1);
		else if (res < 0)
		{
			printf("Error: Arguments can't be negative\n");
			return (1);
		}
		else if (i != 5 && res <= 0)
		{
			printf("Error: Only [n_must_eat] can be 0\n");
			return (1);
		}
		i++;
	}
	asign_args(ac, av, args, &err);
	return (0);
}
