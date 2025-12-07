#include "philo.h"

int pickup_forks(t_philo *philo)
{
    if (philo->props->number_of_philosophers == 1)
    {
        pthread_mutex_lock(&philo->second_fork->fork);
        safe_print(philo, "has taken fork");
        smart_sleep(philo->props->time_to_die, philo);
        pthread_mutex_unlock(&philo->second_fork->fork);
        return 1;
    }
    pthread_mutex_lock(&philo->first_fork->fork);
    if (died_or_ended(philo->props))
    {
        pthread_mutex_unlock(&philo->first_fork->fork);
        return 0;
    }
    safe_print(philo, "has taken fork");
    pthread_mutex_lock(&philo->second_fork->fork);
    if (died_or_ended(philo->props))
    {
        pthread_mutex_unlock(&philo->first_fork->fork);
        pthread_mutex_unlock(&philo->second_fork->fork);
        return 0;
    }
    safe_print(philo, "has taken fork");
    return 2;
}

int eat(t_philo *philo)
{
    int fork_state;

    fork_state = pickup_forks(philo);
    if (fork_state == 0 || fork_state == 1)
        return 0;

    pthread_mutex_lock(&philo->state_lock);
    philo->is_eating = true;
    philo->born_or_last_ate_in_ms = get_ms();
    philo->times_ate += 1;
    pthread_mutex_unlock(&philo->state_lock);

    safe_print(philo, "is eating");
    smart_sleep(philo->props->time_to_eat, philo);

    pthread_mutex_unlock(&philo->first_fork->fork);
    pthread_mutex_unlock(&philo->second_fork->fork);
    return 1;
}

void *live(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    while (1)
    {
        if (died_or_ended(philo->props))
            return NULL;
        if (!eat(philo))
            return NULL;
        if (died_or_ended(philo->props))
            return NULL;
        if (sim_step_or_end(philo))
            return NULL;
        if (died_or_ended(philo->props))
            return NULL;
        philo_sleep(philo);
        safe_print(philo, "is thinking");
        if (philo->props->number_of_philosophers % 2 != 0)
            usleep(300);
    }
    return NULL;
}

int died(t_philo *philo)
{
    long last_ate;
    long now;
    int is_eating;

    now = get_ms();
    pthread_mutex_lock(&philo->state_lock);
    last_ate = philo->born_or_last_ate_in_ms;
    is_eating = philo->is_eating;
    pthread_mutex_unlock(&philo->state_lock);
    if (now - last_ate >= philo->props->time_to_die && !is_eating)
    {
        pthread_mutex_lock(&philo->props->death_lock);
        philo->props->some_philo_died = 1;
        pthread_mutex_unlock(&philo->props->death_lock);
        safe_print(philo, "died");
        return 1;
    }
    return 0;
}

void *track(void *arg)
{
    int i;
    t_monitor *mon;

    mon = (t_monitor *)arg;
    while (1)
    {
        if (died_or_ended(mon->props))
            return NULL;
        i = 0;
        while (i < mon->props->number_of_philosophers)
        {
            if (died(&mon->philos[i]))
                return NULL;
            i++;
        }
        usleep(100);
    }
    return NULL;
}

void philos_init(t_props *props)
{
    int i;
    t_philo *philos;

    philos = props->philos;
    i = -1;
    while (++i < props->number_of_philosophers)
    {
        pthread_mutex_init(&philos[i].state_lock, NULL);
        philos[i].id = i + 1;
        philos[i].times_ate = 0;
        philos[i].is_eating = false;
        philos[i].props = props;
        assign_forks(&philos[i], props->forks, i);
    }
}

void *props_init(t_props *props)
{
    int i;

    props->finished_philos = 0;
    props->all_philos_ready = false;
    props->some_philo_died = false;
    props->simulation_end = false;
    props->start_time = get_ms();
    props->forks = safe_malloc(sizeof(t_fork) * props->number_of_philosophers);
    if (!props->forks)
        return NULL;
    props->philos = safe_malloc(sizeof(t_philo) * props->number_of_philosophers);
    if (!props->philos)
    {
        free(props->forks);
        return NULL;
    }
    i = -1;
    while (++i < props->number_of_philosophers)
    {
        pthread_mutex_init(&props->forks[i].fork, NULL);
        props->forks[i].id = i;
    }
    pthread_mutex_init(&props->death_lock, NULL);
    pthread_mutex_init(&props->print_lock, NULL);
    philos_init(props);
    return props;
}

void run_threads(t_props *props)
{
    t_monitor *monitor;
    void *res;
    int i;
    int total;

    res = props_init(props);
    if (res == NULL)
        return;
    total = props->number_of_philosophers;
    monitor = malloc(sizeof(t_monitor));
    if (!monitor)
    {
        printf("Error: Couldn't allocate memory for monitor\n");
        free(props->philos);
        free(props->forks);
        return;
    }

    i = -1;
    while (++i < total)
        pthread_create(&props->philos[i].thread, NULL, live, (void *)&props->philos[i]);

    monitor->props = props;
    pthread_create(&monitor->tracker, NULL, track, (void *)monitor);

    i = 0;
    while (i < total)
    {
        pthread_join(props->philos[i].thread, NULL);
        i++;
    }
    pthread_join(monitor->tracker, NULL);

    i = -1;
    while (++i < total)
        pthread_mutex_destroy(&props->forks[i].fork);
    pthread_mutex_destroy(&props->death_lock);
    pthread_mutex_destroy(&props->print_lock);

    i = 0;
    while (i < props->number_of_philosophers)
    {
        pthread_mutex_destroy(&props->philos[i].state_lock);
        i++;
    }

    free(props->forks);
    free(props->philos);
    free(monitor);
}