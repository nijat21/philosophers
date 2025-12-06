#include "philo.h"

int pickup_forks(t_philo *philo, int second_fork, int first_fork)
{
    if (second_fork == first_fork)
    {
        pthread_mutex_lock(&philo->forks[second_fork]);
        safe_print(philo, "has taken fork");
        smart_sleep(philo->props->time_to_die, philo);
        pthread_mutex_unlock(&philo->forks[second_fork]);
        return 1;
    }
    pthread_mutex_lock(&philo->forks[first_fork]);
    if (died_or_ended(philo->props))
    {
        pthread_mutex_unlock(&philo->forks[first_fork]);
        return 0;
    }
    safe_print(philo, "has taken fork");
    pthread_mutex_lock(&philo->forks[second_fork]);
    if (died_or_ended(philo->props))
    {
        pthread_mutex_unlock(&philo->forks[first_fork]);
        pthread_mutex_unlock(&philo->forks[second_fork]);
        return 0;
    }
    safe_print(philo, "has taken fork");
    return 2;
}

int eat(t_philo *philo)
{
    int fork_state;
    int first_fork;
    int second_fork;

    assign_forks(philo, &first_fork, &second_fork);
    fork_state = pickup_forks(philo, second_fork, first_fork);
    if (fork_state == 0 || fork_state == 1)
        return 0;
    if (died_or_ended(philo->props))
    {
        pthread_mutex_unlock(&philo->forks[first_fork]);
        pthread_mutex_unlock(&philo->forks[second_fork]);
        return 0;
    }
    pthread_mutex_lock(&philo->state_lock);
    philo->born_or_last_ate_in_ms = get_ms();
    philo->number_of_times_eaten += 1;
    philo->state = EATING;
    pthread_mutex_unlock(&philo->state_lock);
    safe_print(philo, "is eating");
    smart_sleep(philo->props->time_to_eat, philo);
    pthread_mutex_unlock(&philo->forks[first_fork]);
    pthread_mutex_unlock(&philo->forks[second_fork]);
    return 1;
}

void *live(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;

    // initialize last_ate for precise tracking
    pthread_mutex_lock(&philo->state_lock);
    philo->born_or_last_ate_in_ms = get_ms();
    pthread_mutex_unlock(&philo->state_lock);

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
        if (died_or_ended(philo->props))
            return NULL;
        philo_think(philo);
    }
    return NULL;
}

int died(t_philo *philo)
{
    long last_ate;
    t_state curr_state;

    pthread_mutex_lock(&philo->state_lock);
    last_ate = philo->born_or_last_ate_in_ms;
    curr_state = philo->state;
    pthread_mutex_unlock(&philo->state_lock);
    if (get_ms() - last_ate >= philo->props->time_to_die && curr_state != EATING)
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
        usleep(1000);
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
    if (!philos)
    {
        printf("Error: Couldn't allocate memory for philos\n");
        return;
    }
    forks = malloc(sizeof(pthread_mutex_t) * props->number_of_philosophers);
    if (!forks)
    {
        printf("Error: Couldn't allocate memory for forks\n");
        free(philos);
        return;
    }

    i = 0;
    while (i < total)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }
    pthread_mutex_init(&props->death_lock, NULL);
    pthread_mutex_init(&props->print_lock, NULL);

    i = 0;
    while (i < total)
    {
        pthread_mutex_init(&philos[i].state_lock, NULL);
        philos[i].forks = forks;
        philos[i].id = i;
        philos[i].props = props;
        philos[i].number_of_times_eaten = 0;
        philos[i].state = THINKING;
        philos[i].born_or_last_ate_in_ms = props->start_time;
        pthread_create(&philos[i].thread, NULL, live, (void *)&philos[i]);
        usleep(1);
        i++;
    }
    monitor = malloc(sizeof(t_monitor));
    if (!monitor)
    {
        printf("Error: Couldn't allocate memory for monitor\n");
        free(philos);
        free(forks);
        return;
    }
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
    pthread_mutex_destroy(&props->print_lock);

    i = 0;
    while (i < props->number_of_philosophers)
    {
        pthread_mutex_destroy(&philos[i].state_lock);
        i++;
    }

    free(philos);
    free(forks);
    free(monitor);
}