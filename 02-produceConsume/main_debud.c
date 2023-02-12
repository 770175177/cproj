#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

#define MAX_PROD	20

typedef struct a_product
{
    long time;
    char *color;
} prod;

int N;
prod *buffer;
struct timeval current_time;

int count = 0;
int the_end = 0;
int total_prod = 0;

int from_prod = 0;
int stop = 0;

pthread_mutex_t lock;
sem_t white_red, red_blue, blue_white, prod_consume;

void *produce_red(void *arg)
{
    int i;

    while (1)
    {
        sem_wait(&white_red);
        pthread_mutex_lock(&lock);
        if (total_prod == MAX_PROD)
        {
	    the_end = 1;
            break;
        }

        buffer[count].color = (char *)malloc(sizeof(char) * 4);
        buffer[count].color = "red";
        gettimeofday(&current_time, NULL);
        buffer[count].time = current_time.tv_usec;
        fprintf(stdout, "Producer1: %s %ld\n", buffer[count].color, buffer[count].time);

        total_prod++;
        count++;
        from_prod = 0;

        if (count == N)
        {
            for (i = 0; i < N; i++)
            {
                sem_post(&prod_consume);
            }
        }
        else
        {
            sem_post(&red_blue);
        }
        pthread_mutex_unlock(&lock);
    }

    for (i = 0; i < count; i++)
    {
        sem_post(&prod_consume);
    }
    stop = 1;
    if (count == 0)
    {
        sem_post(&prod_consume);
    }
    pthread_mutex_unlock(&lock);

    printf("end producer 1\n");
}

void *produce_blue(void *arg)
{
    int i;

    while (1)
    {
        sem_wait(&red_blue);
        pthread_mutex_lock(&lock);
        if (total_prod == MAX_PROD)
        {
	    the_end = 1;
            break;
        }

        buffer[count].color = (char *)malloc(sizeof(char) * 5);
        buffer[count].color = "blue";
        gettimeofday(&current_time, NULL);
        buffer[count].time = current_time.tv_usec;
        fprintf(stdout, "producer2: %s %ld\n", buffer[count].color, buffer[count].time);

        total_prod++;
        count++;
        from_prod = 1;

        if (count == N)
        {
            for (i = 0; i < N; i++)
            {
                sem_post(&prod_consume);
            }
        }
        else
        {
            sem_post(&blue_white);
        }
        pthread_mutex_unlock(&lock);
    }

    for (i = 0; i < count; i++)
    {
        sem_post(&prod_consume);
    }
    stop = 1;
    if (count == 0)
    {
        sem_post(&prod_consume);
    }
    pthread_mutex_unlock(&lock);

	printf("end produce 2\n");
}

void *produce_white(void *arg)
{
    int i;

    while (1)
    {

        sem_wait(&blue_white);
        pthread_mutex_lock(&lock);
        if (total_prod == 2000)
        {
            break;
        }

        buffer[count].color= (char *)malloc(sizeof(char) * 6);
        buffer[count].color = "white";
        gettimeofday(&current_time, NULL);
        buffer[count].time = current_time.tv_usec;
        fprintf(stdout, "producer3: %s %ld\n", buffer[count].color, buffer[count].time);
        total_prod++;
        count++;
        from_prod = 2;

        if (count == N)
        {
            for (i = 0; i < N; i++)
            {
                sem_post(&prod_consume);
            }
        }
        else
        {
            sem_post(&white_red);
        }
        pthread_mutex_unlock(&lock);
    }

    for (i = 0; i < count; i++)
    {
        sem_post(&prod_consume);
    }
    stop = 1;
    if (count == 0)
    {
        sem_post(&prod_consume);
    }
    pthread_mutex_unlock(&lock);

    printf("end produce 3\n");
}

void *consume1(void *arg)
{
    while (1)
    {
	printf("debug\n");
        sem_wait(&prod_consume);
        pthread_mutex_lock(&lock);

        if (count > 0)
        {
            count--;
            gettimeofday(&current_time, NULL);
            printf("consumer1: %s %ld\n", buffer[count].color, current_time.tv_usec);
        }
	
        if (count == 0)
        {
            if (stop == 1)
            {
                break;
            }

            if (from_prod == 0)
            {
                sem_post(&red_blue);
            }
            else if (from_prod == 1)
            {
                sem_post(&blue_white);
            }
            else
            {
                sem_post(&white_red);
            }
        }
        pthread_mutex_unlock(&lock);
    }
    printf("end consumer 1\n");
}

void *consume2(void *arg)
{
    while (1)
    {
        sem_wait(&prod_consume);
        pthread_mutex_lock(&lock);

        if (count > 0)
        {
            count--;
            gettimeofday(&current_time, NULL);
            printf("consumer2: %s %ld\n", buffer[count].color, current_time.tv_usec);
        }

        if (count == 0)
        {
            if (stop == 1)
            {
                break;
            }

            if (from_prod == 0)
            {
                sem_post(&red_blue);
            }
            else if (from_prod == 1)
            {
                sem_post(&blue_white);
            }
            else
            {
                sem_post(&white_red);
            }
        }
        pthread_mutex_unlock(&lock);
    }

    printf("end consumer 2\n");
}

int main(int argc, char *argv[])
{
    int n;

    pthread_t prod_red, prod_blue, prod_white, cons1, cons2;

    pthread_mutex_init(&lock, NULL);

    if (argc >= 2)
    {
        N = atoi(argv[1]);
    }
    else
    {
        N = 3;
    }
    buffer = (prod *)malloc(sizeof(prod) * N);

    sem_init(&white_red, 0, 1);
    sem_init(&red_blue, 0, 0);
    sem_init(&blue_white, 0, 0);
    sem_init(&prod_consume, 0, 0);

    /*  Create producer thread                        */
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

    if (n = pthread_create(&prod_white, NULL, produce_white, NULL))
    {
        fprintf(stderr, "pthread_create :%s\n", strerror(n));
        exit(1);
    }

    if (n = pthread_create(&cons1, NULL, consume1, NULL))
    {
        fprintf(stderr, "pthread_create :%s\n", strerror(n));
        exit(1);
    }

    /*  Create consumer thread                        */
    if (n = pthread_create(&cons2, NULL, consume2, NULL))
    {
        fprintf(stderr, "pthread_create :%s\n", strerror(n));
        exit(1);
    }

    /* Wait for the consumer 1 thread to finish.         */
    if (n = pthread_join(cons1, NULL))
    {
        fprintf(stderr, "pthread_join:%s\n", strerror(n));
        exit(1);
    }

    /* Wait for the consumer 2 thread to finish.         */
    if (n = pthread_join(cons2, NULL))
    {
        fprintf(stderr, "pthread_join:%s\n", strerror(n));
        exit(1);
    }
    free(buffer);
}
