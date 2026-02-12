#include "philo.h"

typedef struct s_th {
	long id;
	pthread_t t;
} t_th;

typedef struct s_shared {
	t_th *t1;
	t_th *t2;
	long n_ready;
} t_shared;

void *routine(void *arg)
{
	t_shared *sh = (t_shared *)arg;

	sh->n_ready++;
	while(sh->n_ready != 2)
		printf("waiting\n");
	
	printf("Thread  started\n");  
	usleep(100);
	printf("Thread  running\n");
	usleep(100);
	printf("Thread  ended\n");
	return NULL;
}

int main(){

	t_shared *th;

	th = malloc(sizeof(t_shared));
	th->t1 = malloc(sizeof(t_th));
	th->t2 = malloc(sizeof(t_th));
	th->t1->id = 1;
	th->t2->id = 1;

	pthread_create(&th->t1->t, NULL, routine, (void *)th);
	pthread_create(&th->t2->t, NULL, routine, (void *)th);

	pthread_join(th->t1->t, NULL);
	pthread_join(th->t2->t, NULL);
}
