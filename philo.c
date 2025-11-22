#include "philo.h"

/* ToDOs:
    1. Creat a thread for each philosopher with their numbers
    2. Functions to do movements: pick up left fork, pick up right fork, eat, sleep, think
*/

long get_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/*
    1.Fork locks should be shared among phils so that it works
    2.Every fork is a mutex
    3.Which fork can be used by current phil and is locked in that duration
*/

// void *live(void *philo)
// {
//     /*
//         timestamp_in_ms X has taken a fork
//         timestamp_in_ms X has taken a fork
//         timestamp_in_ms X is eating
//         timestamp_in_ms X is sleeping
//         timestamp_in_ms X is thinking
//         timestamp_in_ms X died
//     */
//     t_philos *ph;
//     long start_time;
//     long last_meal;

//     start_time = get_ms();
//     // pthread_mutex_lock();
//     printf("%d has taken a fork\n");
//     // pthread_mutex_lock();
//     printf("%d has taken a fork\n");
//     printf("%d is eating\n");
//     usleep(ph->time_to_eat * 1000);
//     if (get_ms() - start_time == ph->time_to_eat)
//     {
//         printf("%d is sleeping\n");
//         // pthread_mutex_unlock();
//         // pthread_mutex_unlock();
//     }
//     usleep(ph->time_to_sleep * 1000);
//     if (get_ms() - start_time - ph->time_to_eat == ph->time_to_sleep)
//         printf("%d is thinking\n");
//     return NULL;
// }

// t_philos *create_phils(t_philos *philos)
// {
//     int i;
//     pthread_t temp;

//     philos->philos = malloc(philos->number_of_philosophers * sizeof(t_philo));
//     if (!philos->philos)
//     {
//         free(philos);
//         printf("Error: Couldn't allocate memory for philosophers\n");
//         return NULL;
//     }
//     i = 0;
//     while (i < philos->number_of_philosophers)
//     {
//         temp = pthread_create(philos->philos[i].t, NULL, live, (void *)philos);
//         if (temp)
//         {
//             printf("Error: Couldn't create threads\n");
//             free(philos->philos);
//             free(philos);
//             return NULL;
//         }
//         i++;
//     }
//     return philos;
// }

/* int main(int ac, char **av)
{

    t_philos *philos;

    if (!(ac == 5 || ac == 6))
    {
        fprintf(stderr, "Error: program should be run with following arguments ->\n"
                        "./philo number_of_philosophers time_to_die time_to_eat time_to_sleep"
                        "[number_of_times_each_philosopher_must_eat](optional)\n");
        return 0;
    }
    if (ac == 6 && atoi(av[5]) == 0)
    {
        printf("All philosophers have eaten enough!\n");
        return 0;
    }
    philos = malloc(sizeof(t_philos));
    if (!philos)
    {
        return 0;
    }
    int res = check_and_store(ac, av, philos);
    printf("wrong_philos: %d\n", res);
    if (res)
        return 0;
    // printf("number_of_philosophers:%d\n time_to_die:%d\n time_to_eat%d\n time_to_sleep:%d\n"
    //        "[number_of_times_each_philosopher_must_eat](optional):%d\n",
    //        philos->number_of_philosophers,
    //        philos->time_to_die, philos->time_to_eat, philos->time_to_sleep, philos->number_of_times_each_philosopher_must_eat);

    free(philos);
} */

int main()
{
    run_threads();
}
