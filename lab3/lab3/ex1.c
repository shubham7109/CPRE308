// PTHREAD EXAMPLE
// AUTHOR: Shubham Sharma, shubham@iastate.edu

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //Header file for sleep()
#include <pthread.h> //Header file to link pthread_t

// Thread 1 function:
// Prints after 5 secs
void *thread1()
{
	sleep(5);
	printf("Hello,I am thread 1\n");
	return NULL;
}

// Thread 2 function:
// Prints after 5 secs
void *thread2()
{
	sleep(5);
	printf("Hello,I am thread 2\n");
	return NULL;
}

// Main function:
// Creates and runs 2 thread functions.
int main()
{
	pthread_t i1,i2;
	printf("Main: Before Threads\n");
	pthread_create(&i1, NULL, thread1, NULL);
	pthread_create(&i2, NULL, thread2, NULL);
	pthread_join(i1, NULL);
	pthread_join(i2, NULL);
	printf("Main: Threads complete\n");
	exit(0);
}
