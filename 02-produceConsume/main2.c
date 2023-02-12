#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#define BUF_LEN 	20
#define MAX_PROD 	20
#define TWO 	0

typedef struct a_product
{
    long time;
    char *color;
} prod;

prod *buffer;     // loop fifo
int front = 0;
int rear = 0;

int total_prod = 0;

struct timeval current_time;

pthread_mutex_t lock;
sem_t buf_full, buf_empty;

void *produce_red(void *arg)
{
    int i;

    while (1)
    {
        sem_wait(&buf_empty);
        pthread_mutex_lock(&lock);
        if (total_prod == MAX_PROD)
        {
            break;
        }	

	if(((rear+1)%BUF_LEN) != front)   // buf has empty
	{	
       	 	buffer[rear].color = (char *)malloc(sizeof(char) * 4);
        	buffer[rear].color = "red";
        	gettimeofday(&current_time, NULL);
        	buffer[rear].time = current_time.tv_usec;
        	fprintf(stdout, "Producer1: %s %ld\n", buffer[rear].color, buffer[rear].time);
        	total_prod++;
        	rear = (rear+1)%BUF_LEN;
		printf("total_prod=%d, rear=%d, front=%d\n", total_prod, rear, front);
                sem_post(&buf_full);
	}

        pthread_mutex_unlock(&lock);
    }
}

void *produce_blue(void *arg)
{
    int i;

    while (1)
    {
        sem_wait(&buf_empty);
        pthread_mutex_lock(&lock);
        if (total_prod == MAX_PROD)
        {
            break;
        }	

	if(((rear+1)%BUF_LEN) != front)   // buf has empty
	{	
       	 	buffer[rear].color = (char *)malloc(sizeof(char) * 5);
        	buffer[rear].color = "blue";
        	gettimeofday(&current_time, NULL);
        	buffer[rear].time = current_time.tv_usec;
        	fprintf(stdout, "Producer2: %s %ld\n",buffer[rear].color, buffer[rear].time);
        	total_prod++;
        	rear = (rear+1)%BUF_LEN;
		printf("total_prod=%d, rear=%d, front=%d\n", total_prod, rear, front);
                sem_post(&buf_full);
	}

        pthread_mutex_unlock(&lock);
    }
}

void *consume1(void *arg)
{
    while (1)
    {
        sem_wait(&buf_full);
        pthread_mutex_lock(&lock);

	if(front != rear)   // buf is not empty
	{
        	gettimeofday(&current_time, NULL);
        	printf("consumer1: %s %ld\n", buffer[front].color, current_time.tv_usec);
	//	printf("total_prod=%d, rear=%d, front=%d\n", total_prod, rear, front);
		front = (front+1)%BUF_LEN;
                sem_post(&buf_empty);
	}
        pthread_mutex_unlock(&lock);
    }
}

void *consume2(void *arg)
{
    while (1)
    {
        sem_wait(&buf_full);
        pthread_mutex_lock(&lock);

	if(front != rear)   // buf is not empty
	{
        	gettimeofday(&current_time, NULL);
        	printf("consumer2: %s %ld\n", buffer[front].color, current_time.tv_usec);
		printf("total_prod=%d, rear=%d, front=%d\n", total_prod, rear, front);
		front = (front+1)%BUF_LEN;
                sem_post(&buf_empty);
	}
        pthread_mutex_unlock(&lock);
    }
}

int main(int argc, char *argv[])
{
    int n;

    pthread_t prod_red, prod_blue, cons1, cons2;

    pthread_mutex_init(&lock, NULL);

    if (argc >= 2)
    {
        n = atoi(argv[1]);
    }
    else
    {
 	n = BUF_LEN;
    }
    buffer = (prod *)malloc(sizeof(prod) * n);

    sem_init(&buf_full, 0, 0);
    sem_init(&buf_empty, 0, BUF_LEN-1);

    /*  Create producer thread                        */
    if (n = pthread_create(&prod_red, NULL, produce_red, NULL))
    {
        fprintf(stderr, "pthread_create :%s\n", strerror(n));
        exit(1);
    }
#if TWO
    if (n = pthread_create(&prod_blue, NULL, produce_blue, NULL))
    {
        fprintf(stderr, "pthread_create :%s\n", strerror(n));
        exit(1);
    }
#endif

    if (n = pthread_create(&cons1, NULL, consume1, NULL))
    {
        fprintf(stderr, "pthread_create :%s\n", strerror(n));
        exit(1);
    }
#if TWO
    /*  Create consumer thread                        */
    if (n = pthread_create(&cons2, NULL, consume2, NULL))
    {
        fprintf(stderr, "pthread_create :%s\n", strerror(n));
        exit(1);
    }
#endif 

    /* Wait for the consumer 1 thread to finish.         */
    if (n = pthread_join(cons1, NULL))
    {
        fprintf(stderr, "pthread_join:%s\n", strerror(n));
        exit(1);
    }
#if TWO
    /* Wait for the consumer 2 thread to finish.         */
    if (n = pthread_join(cons2, NULL))
    {
        fprintf(stderr, "pthread_join:%s\n", strerror(n));
        exit(1);
    }
#endif
    free(buffer);
}
