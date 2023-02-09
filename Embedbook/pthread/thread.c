#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#define THREAD_NUM 3
#define REPEAT_NUM 5
#define DELAY_TIME_LEVELS 6.0

void *thrd_func(void *arg)
{
	int thrd_num = (int)arg;
	int delay_time = 0;
	int count = 0;
	
	printf("Thread %d id starting\n",thrd_num);
	for(count = 0; count < REPEAT_NUM; count++)
	{
		delay_time = (int)(rand() * DELAY_TIME_LEVELS/ (RAND_MAX)) + 1;
		sleep(delay_time);
		printf("\tThread %d: job %d delay = %d\n",thrd_num, count, delay_time);
	}
	printf("Thread %d finished\n", thrd_num);
	pthread_exit(NULL);
}

int main(void)
{
	pthread_t thread[THREAD_NUM];
	int no = 0, res;
	void * thrd_ret;

	srand(time(NULL));

	for(no = 0; no < THREAD_NUM; no++)
	{
		res = pthread_create(&thread[no], NULL, thrd_func, (void*)no);
		if(res != 0)
		{
			printf("Create thread %d failed\n",no);
			exit(res);
		}
	}
	printf("Create treads success\n Waiting for threads to finish...\n");
	for(no = 0; no < THREAD_NUM; no++)
	{
		res = pthread_join(thread[no], &thrd_ret);
		if(!res)
		{
			printf("Thread %d joined\n", no);
		}
		else
		{
			printf("Thread %d join failed\n", no);
		}
	}

	return 0;
}
