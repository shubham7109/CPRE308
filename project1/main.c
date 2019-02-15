#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 256

char userName[20];
char wholeCommand[MAX_NAME_LENGTH];
int i;
bool nextLine;

void printUserName(char **argv);
void executeCommand();
void printPID();
void printPPID();
void printPWD();
void printCD();
void printSET();
void printGET();

int main(int argc, char** argv) {
	
	while(strcmp(command,"exit") != 0)
	{
		printUserName(argv);
		executeCommand();
	}

	return 0;
}

void executeCommand(){
	fgets(wholeCommand, MAX_NAME_LENGTH, stdin);
	char command = strtok(0,   " ");
	if (strcmp(command, "pid") == 0)
	{
		printPID();
	}

	else if (strcmp(command, "ppid") == 0)
	{
		printPPID();
	}

	else if (strcmp(command, "pwd") == 0)
	{
		printPWD();
	}

	else if (strcmp(command, "cd") == 0)
	{
		printCD();
	}

	else if (strcmp(command, "set") == 0)
	{
		printSET();
	}

	else if (strcmp(command, "get") == 0)
	{
		printGET();
	}

	else
	{
		system(command);
	}

}

void printCD(){
	chdir(getenv("HOME"));

}

void printSET(){
	printf("PID: %d\n",getpid());
}

void printGET(){
	printf("PID: %d\n",getpid());
}

void printPID(){
	printf("PID: %d\n",getpid());
}

void printPPID(){
	printf("PPID: %d\n",getppid());
}

void printPWD(){
	char cwd[MAX_NAME_LENGTH];
	getcwd(cwd,sizeof(cwd));
	printf("%s\n",cwd);
}

void printUserName(char **argv){
	strcpy(userName, "308sh> ");
	if(strcmp(argv[1], "-p") == 0){
		strcpy(userName, argv[2]);

	}

	for(i = 0; i <= strlen(userName); i++)
  	{
  		if(userName[i] == '\"')
		{
  			userName[i] = '\0';
 		}
	}
	printf("%s",userName);
}
