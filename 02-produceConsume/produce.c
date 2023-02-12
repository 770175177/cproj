#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

sem_t full, empty, mutex;
int i = 10;

void *producer()
{
	while(1)
	{
		sem_wait(&mutex);
		sem_wait(&empty);
		printf("produce %d\n", i++);
		sem_post(&mutex);
		sem_post(&full);
	}
}
void *consumer()
{
	while(1)
	{
		sem_wait(&mutex);
		sem_wait(&full);
		printf("consume %d\n", i--);
		sem_post(&mutex);
		sem_post(&empty);
	}
}

int main()
{
	void *ret_p, *ret_c;
	pthread_t pid_p, pid_c;

	sem_init(&full, 0, 0);
	sem_init(&empty, 0, 5);
	sem_init(&mutex, 0, 1);

	pthread_create(&pid_p, NULL, producer, NULL);
	pthread_create(&pid_c, NULL, consumer, NULL);

	pthread_join(pid_p, &ret_p);
	pthread_join(pid_c, &ret_c);

	return 0;
}
