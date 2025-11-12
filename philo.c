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

// ToDos:
/*
1. Check the input
    a. All arguments should be bigger than 0 except last one. If last one is 0, we quit the program
    b. All arguments should be numbers
2. Atoi for converting the args to ints
 */

/*
Arguments:
number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
*/
int main(int ac, char **av)
{
    pthread_t t1;
    pthread_t t2;
    SharedData data;

    if (!(ac == 5 || ac == 6))
    {
        fprintf(stderr, "Error: program should be run with following arguments ->\n"
                        "./philo number_of_philosophers time_to_die time_to_eat time_to_sleep"
                        "[number_of_times_each_philosopher_must_eat](optional)\n");
        return 0;
    }
    if (ac == 6 && atoi(av[5]) == 0)
        return 0;
    if (wrong_args(ac, av))
        return 0;
    data.counter = 0;
    pthread_mutex_init(&data.lock, NULL);

    pthread_create(&t1, NULL, increment, (void *)&data);
    pthread_create(&t2, NULL, increment, (void *)&data);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Counter = %d\n", data.counter);

    pthread_mutex_destroy(&data.lock);

    printf("wrong_args: %d\n", wrong_args(ac, av));
}