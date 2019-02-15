#include <stdio.h>

int main() {
	fork();
	fork();
	printf("Process %d’s parent process ID is %d\n", getpid(), getppid());
	return 0;
}
