#include "philo.h"

long get_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void smart_sleep(long ms)
{
    long start;

    start = get_ms();
    while (get_ms() - start < ms)
        usleep(100);
}

void pickup_forks(t_philo *philo, int left_fork, int right_fork)
{
    if (left_fork == right_fork)
    {
        pthread_mutex_lock(&philo->forks[left_fork]);
        printf(GREEN "[%ld]" RESET " %d has taken fork\n", get_ms() - philo->props->start_time, philo->id + 1);
        smart_sleep(philo->props->time_to_die);
        pthread_mutex_unlock(&philo->forks[left_fork]);
        return;
    }
    else if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->forks[left_fork]);
        pthread_mutex_lock(&philo->props->death_lock);
        if (philo->props->some_philo_died)
        {
            pthread_mutex_unlock(&philo->forks[left_fork]);
            pthread_mutex_unlock(&philo->props->death_lock);
            return;
        }
        pthread_mutex_unlock(&philo->props->death_lock);
        printf(GREEN "[%ld]" RESET " %d has taken fork\n", get_ms() - philo->props->start_time, philo->id + 1);

        pthread_mutex_lock(&philo->props->death_lock);
        if (philo->props->some_philo_died)
        {
            printf("here\n");
            pthread_mutex_unlock(&philo->props->death_lock);
            pthread_mutex_unlock(&philo->forks[left_fork]);
            return;
        }
        pthread_mutex_unlock(&philo->props->death_lock);

        pthread_mutex_lock(&philo->forks[right_fork]);
        pthread_mutex_lock(&philo->props->death_lock);
        if (philo->props->some_philo_died)
        {
            pthread_mutex_unlock(&philo->props->death_lock);
            pthread_mutex_unlock(&philo->forks[left_fork]);
            pthread_mutex_unlock(&philo->forks[right_fork]);
            return;
        }
        pthread_mutex_unlock(&philo->props->death_lock);
        printf(GREEN "[%ld]" RESET " %d has taken fork\n", get_ms() - philo->props->start_time, philo->id + 1);
    }
    else
    {
        pthread_mutex_lock(&philo->forks[right_fork]);
        pthread_mutex_lock(&philo->props->death_lock);
        if (philo->props->some_philo_died)
        {
            pthread_mutex_unlock(&philo->props->death_lock);
            pthread_mutex_unlock(&philo->forks[right_fork]);
            return;
        }
        pthread_mutex_unlock(&philo->props->death_lock);
        printf(GREEN "[%ld]" RESET " %d has taken fork\n", get_ms() - philo->props->start_time, philo->id + 1);
        pthread_mutex_lock(&philo->forks[left_fork]);
        pthread_mutex_lock(&philo->props->death_lock);
        if (philo->props->some_philo_died)
        {
            pthread_mutex_unlock(&philo->props->death_lock);
            pthread_mutex_unlock(&philo->forks[left_fork]);
            pthread_mutex_unlock(&philo->forks[right_fork]);
            return;
        }
        pthread_mutex_unlock(&philo->props->death_lock);
        printf(GREEN "[%ld]" RESET " %d has taken fork\n", get_ms() - philo->props->start_time, philo->id + 1);
    }
}

void eat(t_philo *philo, int left_fork, int right_fork)
{
    pickup_forks(philo, left_fork, right_fork);
    pthread_mutex_lock(&philo->props->death_lock);
    if (philo->props->some_philo_died)
    {
        pthread_mutex_unlock(&philo->props->death_lock);
        pthread_mutex_unlock(&philo->forks[right_fork]);
        pthread_mutex_unlock(&philo->forks[left_fork]);
        return;
    }
    pthread_mutex_unlock(&philo->props->death_lock);
    pthread_mutex_lock(&philo->props->state_lock);
    philo->born_or_last_ate_in_ms = get_ms();
    philo->number_of_times_eaten += 1;
    philo->state = EATING;
    pthread_mutex_unlock(&philo->props->state_lock);
    pthread_mutex_lock(&philo->props->death_lock);
    if (philo->props->some_philo_died)
    {
        pthread_mutex_unlock(&philo->props->death_lock);
        pthread_mutex_unlock(&philo->forks[right_fork]);
        pthread_mutex_unlock(&philo->forks[left_fork]);
        return;
    }
    pthread_mutex_unlock(&philo->props->death_lock);
    printf(GREEN "[%ld]" RESET " %d is eating\n", get_ms() - philo->props->start_time, philo->id + 1);
    smart_sleep(philo->props->time_to_eat);
    if (left_fork != right_fork)
    {
        pthread_mutex_unlock(&philo->forks[right_fork]);
        pthread_mutex_unlock(&philo->forks[left_fork]);
    }
}

void *live(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    int left_fork;
    int right_fork;

    right_fork = philo->id;
    left_fork = (philo->id + 1) % philo->props->number_of_philosophers;
    pthread_mutex_lock(&philo->props->state_lock);
    philo->born_or_last_ate_in_ms = philo->props->start_time;
    pthread_mutex_unlock(&philo->props->state_lock);

    while (1)
    {
        // EAT
        eat(philo, left_fork, right_fork);
        pthread_mutex_lock(&philo->props->state_lock);
        if (philo->number_of_times_eaten == philo->props->number_of_times_each_philosopher_must_eat)
        {
            philo->props->finished_philos += 1;
            if (philo->props->finished_philos == philo->props->number_of_philosophers)
                philo->props->simulation_end = 1;
            pthread_mutex_unlock(&philo->props->state_lock);
            return NULL;
        }
        pthread_mutex_unlock(&philo->props->state_lock);

        pthread_mutex_lock(&philo->props->death_lock);
        if (philo->props->some_philo_died || philo->props->simulation_end)
        {
            pthread_mutex_unlock(&philo->props->death_lock);
            return NULL;
        }
        pthread_mutex_unlock(&philo->props->death_lock);
        pthread_mutex_lock(&philo->props->state_lock);
        philo->state = SLEEPING;
        pthread_mutex_unlock(&philo->props->state_lock);
        printf(GREEN "[%ld]" RESET " %d is sleeping\n", get_ms() - philo->props->start_time, philo->id + 1);
        smart_sleep(philo->props->time_to_sleep);

        pthread_mutex_lock(&philo->props->death_lock);
        if (philo->props->some_philo_died || philo->props->simulation_end)
        {
            pthread_mutex_unlock(&philo->props->death_lock);
            return NULL;
        }
        pthread_mutex_unlock(&philo->props->death_lock);
        pthread_mutex_lock(&philo->props->state_lock);
        philo->state = THINKING;
        pthread_mutex_unlock(&philo->props->state_lock);
        printf(GREEN "[%ld]" RESET " %d is thinking\n", get_ms() - philo->props->start_time, philo->id + 1);
    }
    return NULL;
}

void *track(void *arg)
{
    int i;
    t_monitor *mon;
    long last_ate;
    t_state current_state;

    mon = (t_monitor *)arg;
    while (1)
    {
        pthread_mutex_lock(&mon->props->death_lock);
        if (mon->props->simulation_end)
        {
            pthread_mutex_unlock(&mon->props->death_lock);
            return NULL;
        }
        pthread_mutex_unlock(&mon->props->death_lock);

        i = 0;
        while (i < mon->props->number_of_philosophers)
        {
            pthread_mutex_lock(&mon->props->state_lock);
            last_ate = mon->philos[i].born_or_last_ate_in_ms;
            current_state = mon->philos[i].state;
            pthread_mutex_unlock(&mon->props->state_lock);
            if (get_ms() - last_ate >= mon->props->time_to_die && current_state != EATING)
            {
                printf(RED "[%ld] %d died\n" RESET, get_ms() - mon->props->start_time, mon->philos[i].id + 1);
                pthread_mutex_lock(&mon->props->death_lock);
                mon->props->some_philo_died = 1;
                pthread_mutex_unlock(&mon->props->death_lock);
                return NULL;
            }
            i++;
        }
        usleep(1000);
    }
    return NULL;
}

void run_threads(t_props *props)
{
    t_philo *philos;
    t_monitor *monitor;
    pthread_mutex_t forks[props->number_of_philosophers];
    int i;
    int total;

    props->some_philo_died = 0;
    props->finished_philos = 0;
    props->simulation_end = 0;
    props->start_time = get_ms();

    total = props->number_of_philosophers;
    philos = malloc(sizeof(t_philo) * props->number_of_philosophers);
    i = 0;
    while (i < total)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }

    pthread_mutex_init(&props->death_lock, NULL);
    pthread_mutex_init(&props->state_lock, NULL);

    i = 0;
    while (i < total)
    {
        philos[i].forks = forks;
        philos[i].id = i;
        philos[i].props = props;
        philos[i].number_of_times_eaten = 0;
        philos[i].state = THINKING;
        pthread_create(&philos[i].thread, NULL, live, (void *)&philos[i]);
        usleep(100);
        i++;
    }
    monitor = malloc(sizeof(t_monitor));
    monitor->props = props;
    monitor->philos = philos;
    pthread_create(&monitor->tracker, NULL, track, (void *)monitor);

    i = 0;
    while (i < total)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    pthread_join(monitor->tracker, NULL);

    i = 0;
    while (i < total)
    {
        pthread_mutex_destroy(&forks[i]);
        i++;
    }
    pthread_detach(monitor->tracker);

    // Pthread_detach
    free(philos);
    free(monitor);
}