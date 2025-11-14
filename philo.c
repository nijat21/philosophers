#include "philo.h"

typedef struct
{
    int counter;
    pthread_mutex_t lock;
} SharedData;

void *increment(void *arg)
{
    SharedData *data = (SharedData *)arg;
    pthread_mutex_lock(&data->lock);
    data->counter++;
    pthread_mutex_unlock(&data->lock);
    return NULL;
}

void run_threads()
{
    pthread_t t1;
    pthread_t t2;
    SharedData data;

    data.counter = 0;
    pthread_mutex_init(&data.lock, NULL);

    pthread_create(&t1, NULL, increment, (void *)&data);
    pthread_create(&t2, NULL, increment, (void *)&data);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Counter = %d\n", data.counter);

    pthread_mutex_destroy(&data.lock);
}

/* ToDOs:
    1. Creat a thread for each philosopher with their numbers
    2. Functions to do movements: pick up left fork, pick up right fork, eat, sleep, think
*/

void *routine(void *arg)
{
    /*
        timestamp_in_ms X has taken a fork
        timestamp_in_ms X is eating
        timestamp_in_ms X is sleeping
        timestamp_in_ms X is thinking
        timestamp_in_ms X died
    */
    (void)arg;
    return NULL;
}

t_philos *create_phils(t_philos *philos)
{
    int i;
    pthread_t temp;

    philos->philos = malloc(philos->number_of_philosophers * sizeof(t_philo));
    if (!philos->philos)
    {
        free(philos);
        printf("Error: Couldn't allocate memory for philosophers\n");
        return NULL;
    }
    i = 0;
    // while (i < philos->number_of_philosophers)
    while (i < 1)
    {
        temp = pthread_create(philos->philos[i].t, NULL, routine, (void *)philos->philos[i]);
        if (temp)
        {
            printf("Error: Couldn't create threads\n");
            free(philos->philos);
            free(philos);
            return NULL;
        }
        philos->philos[i].t = temp;
        philos->philos[i].n = i + 1;
        philos->philos[i].time_to_die = philos->time_to_die;
        philos->philos[i].time_to_eat = philos->time_to_eat;
        philos->philos[i].time_to_sleep = philos->time_to_sleep;
        philos->philos[i].number_of_times_eaten = 0;
        i++;
    }
    return philos;
}

int main(int ac, char **av)
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
}