#include<stdio.h>	// std i/o
#include"buffer.h"  	// for buffer 
#include<stdlib.h> 	/* required for rand() */
#include<time.h>
#include<semaphore.h>	// for semaphore locks
#include<unistd.h>	// for sleep, wait etc...
#include<pthread.h>	// for thread creation
sem_t empty;
sem_t full;
pthread_mutex_t buffer_access;
buffer_item item ;
/* the buffer */
buffer_item buffer[BUFFER_SIZE];
int rear=0,front=0;

int *insert_item() { // insertion
	/* insert item into buffer
	return 0 if successful, otherwise

	return -1 indicating an error condition */
	item = GenerateRandom(BUFFER_SIZE);
	pthread_mutex_lock(&buffer_access);
	sem_wait(&empty); 	// not empty anymore
	buffer[rear]=item;
	rear++;
	sem_post(&full);
	pthread_mutex_unlock(&buffer_access);
	return 0;

}

int *remove_item() { // removal
	/* remove an object from buffer placing it in item

	return 0 if successful, otherwise	

	return -1 indicating an error condition */
	item = GenerateRandom(BUFFER_SIZE);
	pthread_mutex_lock(&buffer_access);		
	sem_wait(&full);
	front++;
	sem_post(&empty);
	pthread_mutex_unlock(&buffer_access);
	return 0;
}


int GenerateRandom(int upper) 	// generates random number from 0 to upper
{
	int lower_limit = 0;
	int upper_limit = upper;  
	srand(time(NULL)); 
	int random_num = ( (rand())%( upper_limit - lower_limit ) )  +lower_limit+1 ;
	return random_num;
}



int main(int argc, char *argv) { 
//	usleep(microseconds)
/*	The main() function will be

	passed three parameters on the command line:

	1. How long to sleep before terminating

	2. The number of producer threads

	3. The number of consumer threads

/* 1. Get command line arguments argv[1],argv[2],argv[3] */
	int sleep_duration,num_of_producers,num_of_consumers;	
	argv[1] = sleep_duration;
	printf("%d sleep duration\n");	
	argv[2]	= num_of_producers;
	printf("%d producers\n");
	argv[3] = num_of_consumers;
	printf("%d consumers\n");

	sem_init(&empty,0,1);
	sem_init(&full,0,0);
	pthread_mutex_init(&buffer_access,NULL);
/* 2. Initialize buffer */
	int i;
	for(i=0;i<BUFFER_SIZE;i++)
	{
		buffer[i]=0;
	}
	sem_post(&empty);
	sem_wait(&full);


/* 3. Create producer thread(s) */
	printf("creating producer threads\n");
	pthread_t produce[num_of_producers];
	for(i=0;i<num_of_producers;i++)
	{
		pthread_create(&produce[i],NULL,insert_item,NULL);// creates a new producer
	}


/* 4. Create consumer thread(s) */
	printf("creating consumer threads\n");
	pthread_t consume[num_of_consumers];
	for(i=0;i<num_of_consumers;i++)
	{
		pthread_create(&consume[i],NULL,remove_item,NULL);// creates a new consumer 
	}


/* 5. Sleep */
	
	void *status; 	// will hold status of task of each thread
	for(i=0;i<num_of_producers;i++)
	{
		printf("will sleep for %d ms",sleep_duration);		
		sleep(sleep_duration);
		printf("producer slept for %d ms",sleep_duration);		
		pthread_join(produce[i],&status);
		if(!status)
		{
			printf("\ninsertion success!");
		}
		else
		{
			printf("\ninsertion failed!(buffer FULL)");
		}
	}
	
	for(i=0;i<num_of_consumers;i++)
	{		
		printf("will sleep for %d ms",sleep_duration);
		sleep(sleep_duration);
		printf("consumer slept for %d ms",sleep_duration);		
		pthread_join(consume[i],&status);
		if(!status)
		{
			printf("\nremoval success!");
		}
		else
		{
			printf("\nremoval failed!(buffer EMPTY)");
		}
	}


/* 6. Exit */
	exit(0);	

}


