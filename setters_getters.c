#include "philo.h"

bool get_bool(pthread_mutex_t *lock, bool *val)
{
	bool res;

	pthread_mutex_lock(lock);
	res = *val;
	pthread_mutex_unlock(lock);
	return (res);
}

void set_bool(pthread_mutex_t *lock, bool *val, bool new_val)
{
	pthread_mutex_lock(lock);
	*val = new_val;
	pthread_mutex_unlock(lock);
}

long get_long(pthread_mutex_t *lock, long *val)
{
	long res;

	pthread_mutex_lock(lock);
	res = *val;
	pthread_mutex_unlock(lock);
	return (res);
}

void set_long(pthread_mutex_t *lock, long *val, long new_val)
{
	pthread_mutex_lock(lock);
	*val = new_val;
	pthread_mutex_unlock(lock);
}

void increment_long(pthread_mutex_t *lock, long *val)
{
	pthread_mutex_lock(lock);
	(*val)++;
	pthread_mutex_unlock(lock);
}
