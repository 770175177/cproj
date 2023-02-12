#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct a_product
{
    long time;
    char *color;
} prod;

int N;
prod *buffer;
struct timeval current_time;

int count = 0;
int total_prod = 0;

int from_prod = 0;
int stop = 0;

pthread_mutex_t lock;
sem_t white_red, red_blue, blue_white, prod_consume;

void *produce_red(void *arg)
{
    FILE *fp;
    int i;

    while (1)
    {

        sem_wait(&white_red);
        pthread_mutex_lock(&lock);
        if (total_prod == 2000)
        {
            break;
        }

        buffer[count].color = (char *)malloc(sizeof(char) * 4);
        buffer[count].color = "red";
        gettimeofday(&current_time, NULL);
        buffer[count].time = current_time.tv_usec;
        printf("red: %ld\n", buffer[count].time);
        fp = fopen("producer_red.log", "a+");
        fprintf(fp, "%s %ld\n", buffer[count].color, buffer[count].time);
        fclose(fp);
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
}

void *produce_blue(void *arg)
{
    FILE *fp;
    int i;

    while (1)
    {
        sem_wait(&red_blue);
        pthread_mutex_lock(&lock);
        if (total_prod == 2000)
        {
            break;
        }

        buffer[count].color = (char *)malloc(sizeof(char) * 5);
        buffer[count].color = "blue";
        gettimeofday(&current_time, NULL);
        buffer[count].time = current_time.tv_usec;
        printf("blue: %ld\n", buffer[count].time);
        fp = fopen("producer_blue.log", "a+");
        fprintf(fp, "%s %ld\n", buffer[count].color, buffer[count].time);
        fclose(fp);
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
}

void *produce_white(void *arg)
{
    FILE *fp;
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
        printf("white: %ld\n", buffer[count].time);
        fp = fopen("producer_white.log", "a+");
        fprintf(fp, "%s %ld\n", buffer[count].color, buffer[count].time);
        fclose(fp);
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
}

void *consume(void *arg)
{
    FILE *fp;

    while (1)
    {

        sem_wait(&prod_consume);
        pthread_mutex_lock(&lock);

        if (count > 0)
        {
            count--;
            gettimeofday(&current_time, NULL);
            printf("%s: %ld %ld\n", buffer[count].color,buffer[count].time, current_time.tv_usec);
            if (strcmp(buffer[count].color, "red")==0)
            {
                fp = fopen("cousumer_red.log", "a+");
                fprintf(fp, "%s %ld %ld\n", buffer[count].color, buffer[count].time, current_time.tv_usec);
                fclose(fp);
            }
            else if (strcmp(buffer[count].color, "blue")==0)
            {
                fp = fopen("cousumer_blue.log", "a+");
                fprintf(fp, "%s %ld %ld\n", buffer[count].color, buffer[count].time, current_time.tv_usec);
                fclose(fp);
            }
            else if (strcmp(buffer[count].color, "white")==0)
            {
                fp = fopen("cousumer_white.log", "a+");
                fprintf(fp, "%s %ld %ld\n", buffer[count].color, buffer[count].time, current_time.tv_usec);
                fclose(fp);
            }
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
}

int main(int argc, char *argv[])
{
    int n;

    pthread_t prod_red, prod_blue, prod_white, cons;

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

    /*  Create consumer thread                        */
    if (n = pthread_create(&cons, NULL, consume, NULL))
    {
        fprintf(stderr, "pthread_create :%s\n", strerror(n));
        exit(1);
    }

    /* Wait for the consumer thread to finish.         */
    if (n = pthread_join(cons, NULL))
    {
        fprintf(stderr, "pthread_join:%s\n", strerror(n));
        exit(1);
    }
    free(buffer);
}
