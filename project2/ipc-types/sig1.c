#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define SIGNAL 42

int send_signal (int pid)
{
        int ret;
        ret = kill(pid,SIGNAL);
        printf("Sending signal... \n",ret);

}

int main(int argc, char** argv)
{
	int pid=0;
	while(1){
		printf("Enter pid:");
	  scanf("%d",&pid);
	  if(pid != 0)
	    send_signal(pid);
	}
	return 0;
}
