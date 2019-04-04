#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define SIGNAL 42

void my_handler(int i)
{
  printf("\nI got the signal: %d!\n",i );
}

int main(int argc, char** argv)
{
	signal(SIGNAL, my_handler);
  printf("PID of this process is: %d\n", getpid());
	while(1)
	{
		printf(".");
		fflush(stdout);
		sleep(1);
	}
	return 0;
}
