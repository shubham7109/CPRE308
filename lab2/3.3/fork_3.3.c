#include <stdio.h>

int main() {
	int ret;
	ret = fork();
	printf("The ret value: %d\n", ret);
	if (ret == 0) {
	/* this is the child process */
	printf("The child process ID is %d\n", getpid());
	printf("The child’s parent process ID is %d\n", getppid());
	} else {
	/* this is the parent process */
	printf("The parent process ID is %d\n", getpid());
	printf("The parent’s parent process ID is %d\n", getppid());
	}
	sleep(2);
}
