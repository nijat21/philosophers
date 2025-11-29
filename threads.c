#include "philo.h"

long get_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void pickup_forks(t_philo *philo, int left_fork, int right_fork)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->forks[left_fork]);
        printf("%ld %d has taken fork\n", get_ms() - philo->start, philo->id + 1);
        philo->left_fork_picked = 1;
        pthread_mutex_lock(&philo->forks[right_fork]);
        printf("%ld %d has taken fork\n", get_ms() - philo->start, philo->id + 1);
        philo->right_fork_picked = 1;
    }
    else
    {
        pthread_mutex_lock(&philo->forks[right_fork]);
        printf("%ld %d has taken fork\n", get_ms() - philo->start, philo->id + 1);
        philo->right_fork_picked = 1;
        if (left_fork < philo->props->number_of_philosophers)
        {
            pthread_mutex_lock(&philo->forks[left_fork]);
            printf("%ld %d has taken fork\n", get_ms() - philo->start, philo->id + 1);
            philo->left_fork_picked = 1;
        }
        else
            return;
    }
}

void eat(t_philo *philo, int left_fork, int right_fork)
{
    if (philo->left_fork_picked && philo->right_fork_picked)
    {
        printf("%ld %d is eating\n", get_ms() - philo->start, philo->id + 1);
        usleep(philo->props->time_to_eat * 1000);
        philo->number_of_times_eaten += 1;
        philo->born_or_last_ate_in_ms = get_ms();

        philo->right_fork_picked = 0;
        pthread_mutex_unlock(&philo->forks[right_fork]);
    }
    philo->left_fork_picked = 0;
    pthread_mutex_unlock(&philo->forks[left_fork]);
}

void *live(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    int left_fork;
    int right_fork;
    int i;

    right_fork = philo->id;
    left_fork = (philo->id + 1) % philo->props->number_of_philosophers;
    philo->start = get_ms();
    philo->born_or_last_ate_in_ms = get_ms();

    i = 0;
    while (i < philo->props->number_of_times_each_philosopher_must_eat)
    {
        // another condition checking if there're enough forks or time to eat
        // phiolo should at least pickup the fork even if ther's one fork
        // philo should pick up forks even if there's not time to eat
        if (philo->number_of_times_eaten == philo->props->number_of_times_each_philosopher_must_eat)
        {
            pthread_mutex_lock(&philo->props->state_update_lock);
            philo->props->finished_philos += 1;
            if (philo->props->finished_philos == philo->props->number_of_philosophers)
                philo->props->simulation_end = 1;
            pthread_mutex_unlock(&philo->props->state_update_lock);
            return NULL;
        }
        if (get_ms() - philo->born_or_last_ate_in_ms >= philo->props->time_to_die)
        {
            printf("now - born or last ate: %ld time to die: %d\n", get_ms() - philo->born_or_last_ate_in_ms, philo->props->time_to_die);
            pthread_mutex_lock(&philo->props->death_lock);
            printf("%ld %d died\n", get_ms() - philo->start, philo->id + 1);
            philo->props->some_philo_died = 1;
            pthread_mutex_unlock(&philo->props->death_lock);
            return NULL;
        }
        pickup_forks(philo, left_fork, right_fork);
        eat(philo, left_fork, right_fork);
        if (get_ms() - philo->born_or_last_ate_in_ms >= philo->props->time_to_die)
        {
            printf("now - born or last ate: %ld time to die: %d\n", get_ms() - philo->born_or_last_ate_in_ms, philo->props->time_to_die);
            pthread_mutex_lock(&philo->props->death_lock);
            printf("%ld %d died\n", get_ms() - philo->start, philo->id + 1);
            philo->props->some_philo_died = 1;
            pthread_mutex_unlock(&philo->props->death_lock);
            return NULL;
        }
        if (philo->props->some_philo_died)
            return NULL;
        printf("%ld %d is sleeping\n", get_ms() - philo->start, philo->id + 1);
        usleep(philo->props->time_to_sleep * 1000);
        printf("%ld %d is thinking\n", get_ms() - philo->start, philo->id + 1);

        i++;
    }
    return NULL;
}

void run_threads(t_props *props)
{
    t_philo *philos;
    pthread_mutex_t forks[props->number_of_philosophers];
    int i;
    int total;

    props->some_philo_died = 0;
    props->finished_philos = 0;
    props->simulation_end = 0;

    total = props->number_of_philosophers;
    philos = malloc(sizeof(t_philo) * props->number_of_philosophers);
    i = 0;
    while (i < total)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }

    pthread_mutex_init(&props->death_lock, NULL);
    pthread_mutex_init(&props->state_update_lock, NULL);

    i = 0;
    while (i < total)
    {
        philos[i].forks = forks;
        philos[i].id = i;
        philos[i].props = props;
        philos[i].number_of_times_eaten = 0;
        philos[i].left_fork_picked = 0;
        philos[i].right_fork_picked = 0;
        pthread_create(&philos[i].thread, NULL, live, (void *)&philos[i]);
        i++;
        if (props->simulation_end || props->some_philo_died)
        {
            total = i;
            break;
        }
    }

    i = 0;
    while (i < total)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }

    i = 0;
    while (i < total)
    {
        pthread_mutex_destroy(&forks[i]);
        i++;
    }
}