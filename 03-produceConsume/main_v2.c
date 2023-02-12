#include<stdio.h>
#include<pthread.h> 
#include<semaphore.h> 

#define BUFSIZE 6 
#define MAX_PRODUCE 4000 

struct prodcons {
	int buffer[BUFSIZE]; 
	pthread_mutex_t lock;
	int readpos , writepos; 
	pthread_cond_t notempty; 
	pthread_cond_t notfull; 
};

int n=1,m=1;
int total_produce=0, stop=0;
sem_t consume_1to2, consume_2to1;
struct prodcons buffer;

void init(struct prodcons *b){
	sem_init(&consume_1to2, 0, 1);
	sem_init(&consume_2to1, 0, 0);
	pthread_mutex_init(&b->lock,NULL);
	pthread_cond_init(&b->notempty,NULL);
	pthread_cond_init(&b->notfull,NULL);
	b->readpos=0;
	b->writepos=0;
}

void *producer1(void *data){
	
    while(1){	
	pthread_mutex_lock(&(buffer.lock));       
 
	if(((buffer.writepos + 1) % BUFSIZE) == buffer.readpos){
		pthread_cond_wait(&(buffer.notfull), &(buffer.lock));
	}

        printf("producer1-----: ");
	printf("put red to buffer\n");
	buffer.buffer[buffer.writepos]=n;

	buffer.writepos++;
	if(buffer.writepos >= BUFSIZE)
		buffer.writepos=0;
        
	pthread_cond_signal(&(buffer.notempty));
	pthread_mutex_unlock(&(buffer.lock));
        n++;
	total_produce++;
	if(total_produce >= MAX_PRODUCE)
	{
		stop = 1;
		break;
	}
    }	
}

void *producer2(void *data){
	
    while(1){	
	pthread_mutex_lock(&(buffer.lock));       
 
	if(((buffer.writepos + 1) % BUFSIZE) == buffer.readpos){
		pthread_cond_wait(&(buffer.notfull), &(buffer.lock));
	}

        printf("producer2-----: ");
	printf("put blue to buffer\n");
	buffer.buffer[buffer.writepos]=-n;

	buffer.writepos++;
	if(buffer.writepos >= BUFSIZE)
		buffer.writepos=0;
        
	pthread_cond_signal(&(buffer.notempty));
	pthread_mutex_unlock(&(buffer.lock));
        n++;
	total_produce++;
	if(total_produce >= MAX_PRODUCE)
	{
		stop = 1;
		break;
	}
    }	
}

void *consumer1(void *data){
    int d;
    while(1){
       
	sem_wait(&consume_1to2); 
	pthread_mutex_lock(&(buffer.lock));

	if(buffer.writepos == buffer.readpos){
		pthread_cond_wait(&(buffer.notempty), &(buffer.lock));
	}

	d = buffer.buffer[buffer.readpos];
 	printf("first consumer: ");
	if(d > 0)
		printf("get data from buffer is red\n");
	else
		printf("get data from buffer is blue\n");
		
	buffer.readpos++;
	if(buffer.readpos >= BUFSIZE)
		buffer.readpos=0;
       
	pthread_cond_signal(&(buffer.notfull));
	sem_post(&consume_2to1);
	pthread_mutex_unlock(&(buffer.lock));
	if(stop && (buffer.writepos == buffer.readpos))
		break;
    }
    return NULL;
}

void *consumer2(void *data){
    int d;
    while(1){
        sem_wait(&consume_2to1);
	pthread_mutex_lock(&(buffer.lock));

	if(buffer.writepos == buffer.readpos){
		pthread_cond_wait(&(buffer.notempty), &(buffer.lock));
	}

	d = buffer.buffer[buffer.readpos];
 	printf("second consumer: ");
	if(d > 0)
		printf("get data from buffer is red\n");
	else
		printf("get data from buffer is blue\n");
		
	buffer.readpos++;
	if(buffer.readpos >= BUFSIZE)
		buffer.readpos=0;
       
	pthread_cond_signal(&(buffer.notfull));
	sem_post(&consume_1to2);
	pthread_mutex_unlock(&(buffer.lock));
	if(stop && (buffer.writepos == buffer.readpos))
		break;
    }
    return NULL;
}

int main(void){
	pthread_t th_a, th_b;
	void *retval;

	init(&buffer);

	pthread_create(&th_a, NULL, producer1, 0);
	pthread_create(&th_a, NULL, producer2, 0);
	pthread_create(&th_a, NULL, consumer1, 0);
	pthread_create(&th_b, NULL, consumer2, 0);

	pthread_join(th_a, &retval);
	pthread_join(th_b, &retval);	

	return 0;
}
