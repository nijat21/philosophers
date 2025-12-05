#include "philo.h"

long get_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void smart_sleep(long ms, t_philo *philo)
{
    long start;
    int philo_died;

    start = get_ms();
    while (get_ms() - start < ms)
    {
        pthread_mutex_lock(&philo->props->death_lock);
        philo_died = philo->props->some_philo_died;
        pthread_mutex_unlock(&philo->props->death_lock);
        if (philo_died)
            break;
        usleep(100);
    }
}

void safe_print(t_philo *philo, char *msg)
{
    pthread_mutex_lock(&philo->props->print_lock);
    pthread_mutex_lock(&philo->props->death_lock);
    if (!philo->props->some_philo_died)
    {
        // usleep(150);
        if (strcmp(msg, "died") == 0)
            printf(RED "[%ld]" RESET " %d %s\n", get_ms() - philo->props->start_time, philo->id + 1, msg);
        else
            printf(GREEN "[%ld]" RESET " %d %s\n", get_ms() - philo->props->start_time, philo->id + 1, msg);
    }
    pthread_mutex_unlock(&philo->props->death_lock);
    pthread_mutex_unlock(&philo->props->print_lock);
}

int pickup_forks(t_philo *philo, int left_fork, int right_fork)
{
    if (left_fork == right_fork)
    {
        pthread_mutex_lock(&philo->forks[left_fork]);
        safe_print(philo, "has taken fork");
        smart_sleep(philo->props->time_to_die, philo);
        pthread_mutex_unlock(&philo->forks[left_fork]);
        return 1;
    }
    pthread_mutex_lock(&philo->forks[right_fork]);
    pthread_mutex_lock(&philo->props->death_lock);
    if (philo->props->some_philo_died)
    {
        pthread_mutex_unlock(&philo->props->death_lock);
        pthread_mutex_unlock(&philo->forks[right_fork]);
        return 0;
    }
    pthread_mutex_unlock(&philo->props->death_lock);
    safe_print(philo, "has taken fork");
    pthread_mutex_lock(&philo->forks[left_fork]);
    pthread_mutex_lock(&philo->props->death_lock);
    if (philo->props->some_philo_died)
    {
        pthread_mutex_unlock(&philo->props->death_lock);
        pthread_mutex_unlock(&philo->forks[left_fork]);
        pthread_mutex_unlock(&philo->forks[right_fork]);
        return 0;
    }
    pthread_mutex_unlock(&philo->props->death_lock);
    safe_print(philo, "has taken fork");
    return 2;
}

void eat(t_philo *philo, int left_fork, int right_fork)
{
    int philo_died;
    int fork_state;

    fork_state = pickup_forks(philo, left_fork, right_fork);
    if (fork_state == 0 || fork_state == 1)
        return;
    pthread_mutex_lock(&philo->props->death_lock);
    philo_died = philo->props->some_philo_died;
    pthread_mutex_unlock(&philo->props->death_lock);
    if (philo_died)
    {
        if (fork_state == 2)
        {
            pthread_mutex_unlock(&philo->forks[right_fork]);
            pthread_mutex_unlock(&philo->forks[left_fork]);
        }
        return;
    }
    pthread_mutex_lock(&philo->props->state_lock);
    philo->born_or_last_ate_in_ms = get_ms();
    philo->number_of_times_eaten += 1;
    philo->state = EATING;
    pthread_mutex_unlock(&philo->props->state_lock);
    safe_print(philo, "is eating");
    smart_sleep(philo->props->time_to_eat, philo);
    pthread_mutex_unlock(&philo->forks[right_fork]);
    pthread_mutex_unlock(&philo->forks[left_fork]);
}

void *live(void *arg)
{
    t_philo *philo = (t_philo *)arg;
    int left_fork;
    int right_fork;
    int died_or_ended;

    // right_fork = min(philo->id, (philo->id + 1) % philo->props->number_of_philosophers);
    // left_fork = max(philo->id, (philo->id + 1) % philo->props->number_of_philosophers);
    if (philo->id % 2 == 0)
    {
        right_fork = (philo->id + 1) % philo->props->number_of_philosophers;
        left_fork = philo->id;
    }
    else
    {
        right_fork = philo->id;
        left_fork = (philo->id + 1) % philo->props->number_of_philosophers;
    }

    while (1)
    {
        eat(philo, left_fork, right_fork);
        pthread_mutex_lock(&philo->props->state_lock);
        if (philo->props->number_of_times_each_philosopher_must_eat != -1 &&
            philo->number_of_times_eaten == philo->props->number_of_times_each_philosopher_must_eat)
        {
            philo->props->finished_philos += 1;
            if (philo->props->finished_philos == philo->props->number_of_philosophers)
            {
                pthread_mutex_lock(&philo->props->death_lock);
                philo->props->simulation_end = 1;
                pthread_mutex_unlock(&philo->props->death_lock);
            }
            pthread_mutex_unlock(&philo->props->state_lock);
            return NULL;
        }
        pthread_mutex_unlock(&philo->props->state_lock);

        pthread_mutex_lock(&philo->props->death_lock);
        died_or_ended = philo->props->some_philo_died || philo->props->simulation_end;
        pthread_mutex_unlock(&philo->props->death_lock);
        if (died_or_ended)
            return NULL;

        pthread_mutex_lock(&philo->props->state_lock);
        if (philo->state == EATING)
        {
            philo->state = SLEEPING;
            safe_print(philo, "is sleeping");
            pthread_mutex_unlock(&philo->props->state_lock);
            smart_sleep(philo->props->time_to_sleep, philo);
        }
        else
        {
            pthread_mutex_unlock(&philo->props->state_lock);
            return NULL;
        }

        pthread_mutex_lock(&philo->props->death_lock);
        died_or_ended = philo->props->some_philo_died || philo->props->simulation_end;
        pthread_mutex_unlock(&philo->props->death_lock);
        if (died_or_ended)
            return NULL;

        pthread_mutex_lock(&philo->props->state_lock);
        philo->state = THINKING;
        pthread_mutex_unlock(&philo->props->state_lock);
        safe_print(philo, "is thinking");
    }
    return NULL;
}

void *track(void *arg)
{
    int i;
    t_monitor *mon;
    long last_ate;
    t_state current_state;
    int sim_end;

    mon = (t_monitor *)arg;
    while (1)
    {
        pthread_mutex_lock(&mon->props->death_lock);
        sim_end = mon->props->simulation_end;
        pthread_mutex_unlock(&mon->props->death_lock);
        if (sim_end)
            return NULL;

        i = 0;
        while (i < mon->props->number_of_philosophers)
        {
            pthread_mutex_lock(&mon->props->state_lock);
            last_ate = mon->philos[i].born_or_last_ate_in_ms;
            current_state = mon->philos[i].state;
            pthread_mutex_unlock(&mon->props->state_lock);
            if (get_ms() - last_ate >= mon->props->time_to_die && current_state != EATING)
            {
                safe_print(&mon->philos[i], "died");
                pthread_mutex_lock(&mon->props->death_lock);
                mon->props->some_philo_died = 1;
                pthread_mutex_unlock(&mon->props->death_lock);
                return NULL;
            }
            i++;
        }
        // usleep(100);
    }
    return NULL;
}

void run_threads(t_props *props)
{
    t_philo *philos;
    t_monitor *monitor;
    pthread_mutex_t *forks;
    int i;
    int total;

    props->some_philo_died = 0;
    props->finished_philos = 0;
    props->simulation_end = 0;
    props->start_time = get_ms();

    total = props->number_of_philosophers;
    philos = malloc(sizeof(t_philo) * props->number_of_philosophers);
    forks = malloc(sizeof(pthread_mutex_t) * props->number_of_philosophers);
    i = 0;
    while (i < total)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }
    pthread_mutex_init(&props->death_lock, NULL);
    pthread_mutex_init(&props->state_lock, NULL);
    pthread_mutex_init(&props->print_lock, NULL);

    i = 0;
    while (i < total)
    {
        philos[i].forks = forks;
        philos[i].id = i;
        philos[i].props = props;
        philos[i].number_of_times_eaten = 0;
        philos[i].state = THINKING;
        philos[i].born_or_last_ate_in_ms = props->start_time;
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
    pthread_mutex_destroy(&props->death_lock);
    pthread_mutex_destroy(&props->state_lock);
    pthread_mutex_destroy(&props->print_lock);

    // Pthread_detach
    free(forks);
    free(philos);
    free(monitor);
}