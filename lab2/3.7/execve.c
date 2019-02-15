#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

main() {
	execl("/bin/pwd", "pwd", NULL);
	printf("What happened?\n");
}
