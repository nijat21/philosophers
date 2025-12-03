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

    t_props *props;

    if (!(ac == 5 || ac == 6))
    {
        fprintf(stderr, "Error: program should be run with following arguments ->\n"
                        "./philo number_of_philosophers time_to_die time_to_eat time_to_sleep"
                        "[number_of_times_each_philosopher_must_eat](optional)\n");
        return 0;
    }
    if (ac == 6 && atoi(av[5]) == 0)
    {
        printf(GREEN "All philosophers have eaten enough!\n" RESET);
        return 0;
    }
    props = malloc(sizeof(t_props));
    if (!props)
    {
        printf("Memory couldn't be allocated\n");
        return 0;
    }
    int res = check_and_store(ac, av, props);
    // printf("wrong_philos: %d\n", res);
    if (res)
    {
        free(props);
        return 0;
    }
    // printf("number_of_philosophers:%d\n time_to_die:%d\n time_to_eat%d\n time_to_sleep:%d\n"
    //        "[number_of_times_each_philosopher_must_eat](optional):%d\n",
    //        philos->number_of_philosophers,
    //        philos->time_to_die, philos->time_to_eat, philos->time_to_sleep, philos->number_of_times_each_philosopher_must_eat);

    run_threads(props);
    free(props);
}
