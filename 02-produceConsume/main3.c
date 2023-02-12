#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODOCUT 	40
#define BUF_SIZE	5

typedef struct _product{
	long time;
	char *color;
} Product;

typedef struct _config{
	Product buf[BUF_SIZE];
	int front;
	int rear;
	int total_product;
	int producer_complete;
	struct timeval current_time;
	sem_t full;
	sem_t empty;
	sem_t mutex;
}CFG;

CFG config;
CFG *cfg = &config;

void init_all(CFG *cfg)
{
	cfg->front = 0;
	cfg->rear = 0;
	cfg->total_product = 0;
	cfg->producer_complete = 0;
	
	sem_init(&cfg->full, 0, 0);
	sem_init(&cfg->empty, 0, BUF_SIZE-1);
	sem_init(&cfg->mutex, 0, 1);
}

void *producer1()
{
	Product p;
	struct timeval t;

	while(1)
	{
		gettimeofday(&t, NULL);
		p.color = (char *)malloc(sizeof(char) * 4);
		p.color = "red";
		p.time = t.tv_usec;

		sem_wait(&cfg->empty);
		sem_wait(&cfg->mutex);
		
		printf("producer 1 --- : put into buffer(color:%s, time:%ld)\n", p.color, p.time);
		cfg->buf[cfg->rear] = p;
		cfg->rear = (cfg->rear+1)%BUF_SIZE;

		sem_post(&cfg->mutex);
		sem_post(&cfg->full);
		
		cfg->total_product++;
		if(cfg->total_product >= MAX_PRODOCUT)
		{
			cfg->producer_complete = 1;
			break;
		}
	}
}

void *producer2()
{
	Product p;
	struct timeval t;	

	while(1)
	{
		gettimeofday(&t, NULL);
		p.color = (char *)malloc(sizeof(char) * 5);
		p.color = "blue";
		p.time = t.tv_usec;

		sem_wait(&cfg->empty);
		sem_wait(&cfg->mutex);
		
		printf("producer 2 --- : put into buffer(color:%s, time:%ld)\n", p.color, p.time);
		cfg->buf[cfg->rear] = p;
		cfg->rear = (cfg->rear+1)%BUF_SIZE;

		sem_post(&cfg->mutex);
		sem_post(&cfg->full);
		
		cfg->total_product++;
		if(cfg->total_product >= MAX_PRODOCUT)
		{
			cfg->producer_complete = 1;
			break;
		}
	}
}

void *consumer1()
{
	Product p;	

	while(1)
	{
		sem_wait(&cfg->full);
		sem_wait(&cfg->mutex);

		p = cfg->buf[cfg->front];
		cfg->front = (cfg->front)%BUF_SIZE;

		sem_post(&cfg->mutex);
		sem_post(&cfg->empty);

		printf("first consumer, (color: %s, time: %ld)\n", p.color, p.time);

		if(cfg->producer_complete && (1))
		{
			break;
		}
	}
}

void *consumer2()
{
	Product p;

	while(1)
	{
		sem_wait(&cfg->full);
		sem_wait(&cfg->mutex);

		p = cfg->buf[cfg->front];
		cfg->front = (cfg->front)%BUF_SIZE;

		sem_post(&cfg->mutex);
		sem_post(&cfg->empty);

		printf("second consumer, (color: %s, time: %ld)\n", p.color, p.time);

		if(cfg->producer_complete && (1))
		{
			break;
		}
	}
}

int main()
{
	void *ret;
	pthread_t pid_p1, pid_p2, pid_c1, pid_c2;

	init_all(cfg);	

	pthread_create(&pid_p1, NULL, producer1, NULL);
	pthread_create(&pid_p2, NULL, producer2, NULL);
	pthread_create(&pid_c1, NULL, consumer1, NULL);
	pthread_create(&pid_c2, NULL, consumer2, NULL);

	pthread_join(pid_c1, &ret);
	pthread_join(pid_c2, &ret);

	return 0;
}
