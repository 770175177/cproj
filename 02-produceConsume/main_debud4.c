#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#define BUF_LEN 	20
#define MAX_PRODUCE 	20
#define TWO	 	1

typedef struct a_product{
    long time;
    char *color;
}Product;

typedef struct queue{
    Product *buffer;
    int front;
    int rear;
}Queue;

int total_prod = 0;
int the_end = 0;

struct timeval current_time;

sem_t buf_full, buf_empty, mutex;

Queue buf_queue;

void *produce_red(void *arg)
{
    while (1)
    {
        sem_wait(&buf_empty);
	sem_wait(&mutex);

	buf_queue.buffer[buf_queue.rear].color = (char *)malloc(sizeof(char) * 4);
	buf_queue.buffer[buf_queue.rear].color = "red";

	gettimeofday(&current_time, NULL);
	buf_queue.buffer[buf_queue.rear].time = current_time.tv_usec;

	fprintf(stdout, "Producer1: %s %ld\n", buf_queue.buffer[buf_queue.rear].color, buf_queue.buffer[buf_queue.rear].time);
	buf_queue.rear = (buf_queue.rear+1)%BUF_LEN;

	sem_post(&mutex);
	sem_post(&buf_full);
	total_prod++;
	if(total_prod >= MAX_PRODUCE)
	{
		the_end = 1;
		break;
	}
    }
}

void *produce_blue(void *arg)
{
    while (1)
    {
        sem_wait(&buf_empty);
	sem_wait(&mutex);

	buf_queue.buffer[buf_queue.rear].color = (char *)malloc(sizeof(char) * 5);
	buf_queue.buffer[buf_queue.rear].color = "blue";

	gettimeofday(&current_time, NULL);
	buf_queue.buffer[buf_queue.rear].time = current_time.tv_usec;

	fprintf(stdout, "Producer2: %s %ld\n", buf_queue.buffer[buf_queue.rear].color, buf_queue.buffer[buf_queue.rear].time);
	buf_queue.rear = (buf_queue.rear+1)%BUF_LEN;

	sem_post(&mutex);
	sem_post(&buf_full);
	total_prod++;
	if(total_prod >= MAX_PRODUCE)
	{
		the_end = 1;
		break;
	}
    }
}

void *consume1(void *arg)
{
    while (1)
    {
        sem_wait(&buf_full);
        sem_wait(&mutex);

	printf("consumer1: %s %ld\n", buf_queue.buffer[buf_queue.front].color, buf_queue.buffer[buf_queue.front].time);
	buf_queue.front = (buf_queue.front+1)%BUF_LEN;

	sem_post(&mutex);
	sem_post(&buf_empty);

	if(the_end && (buf_queue.rear == buf_queue.front))
		break;
    }
}

void *consume2(void *arg)
{
    while (1)
    {
        sem_wait(&buf_full);
        sem_wait(&mutex);

	printf("consumer2: %s %ld\n", buf_queue.buffer[buf_queue.front].color, buf_queue.buffer[buf_queue.front].time);
	buf_queue.front = (buf_queue.front+1)%BUF_LEN;

	sem_post(&mutex);
	sem_post(&buf_empty);

	if(the_end && (buf_queue.rear == buf_queue.front))
		break;
    }
}

int main(int argc, char *argv[])
{
    int n;

    pthread_t prod_red, prod_blue, cons1, cons2;

    if (argc >= 2)
    {
        n = atoi(argv[1]);
    }
    else
    {
 	n = BUF_LEN;
    }
    buf_queue.buffer = (Product *)malloc(sizeof(Product) * BUF_LEN);
    buf_queue.rear = 0;
    buf_queue.front = 0;

    sem_init(&buf_full, 0, 0);
    sem_init(&buf_empty, 0, BUF_LEN-1);
    sem_init(&mutex, 0, 1);

    /*  Create producer thread */
    if (n = pthread_create(&prod_red, NULL, produce_red, NULL))
    {
        fprintf(stderr, "pthread_create :%s\n", strerror(n));
        exit(1);
    }

    if (n = pthread_create(&prod_blue, NULL, produce_blue, NULL))
    {
        fprintf(stderr, "pthread_create :%s\n", strerror(n));
        exit(1);
    }

    /*  Create consumer thread */
    if (n = pthread_create(&cons1, NULL, consume1, NULL))
    {
        fprintf(stderr, "pthread_create :%s\n", strerror(n));
        exit(1);
    }
#if TWO
    if (n = pthread_create(&cons2, NULL, consume2, NULL))
    {
        fprintf(stderr, "pthread_create :%s\n", strerror(n));
        exit(1);
    }
#endif 

    /* Wait for the consumer thread to finish. */
    if (n = pthread_join(cons1, NULL))
    {
        fprintf(stderr, "pthread_join:%s\n", strerror(n));
        exit(1);
    }
#if TWO
    if (n = pthread_join(cons2, NULL))
    {
        fprintf(stderr, "pthread_join:%s\n", strerror(n));
        exit(1);
    }
#endif
    free(buf_queue.buffer);
}
