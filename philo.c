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

int main(int ac, char **av)
{
    int res;
    t_props *props;

    if (!(ac == 5 || ac == 6))
    {
        printf(RED "Error: program should be run with following arguments ->\n"
                   "./philo number_of_philosophers time_to_die time_to_eat time_to_sleep"
                   " [number_of_times_each_philosopher_must_eat](optional)\n" RESET);
        return 0;
    }
    if (ac == 6 && atoi(av[5]) == 0)
    {
        printf(GREEN "All philosophers have eaten enough!\n" RESET);
        return 0;
    }
    props = safe_malloc(sizeof(t_props));
    if (!props)
        return 0;

    res = check_and_store(ac, av, props);
    if (res)
    {
        free(props);
        return 0;
    }

    run_threads(props);
    free(props);
}
