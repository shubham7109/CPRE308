#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 256

char userName[20];
char wholeCommand[MAX_NAME_LENGTH];
char* args[MAX_NAME_LENGTH];
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
void buildArgs(char** list);

int main(int argc, char** argv) {

	//printf("lol\n");
	while(1)
	{
		printUserName(argv);
		executeCommand();
	}

	return 0;
}

void executeCommand(){
	buildArgs(args);

	if (strcmp(args[0], "pid") == 0)
	{
		printPID();
	}

	else if (strcmp(args[0], "ppid") == 0)
	{
		printPPID();
	}

	else if (strcmp(args[0], "pwd") == 0)
	{
		printPWD();
	}

	else if (strcmp(args[0], "cd") == 0)
	{
		printCD();
	}

	else if (strcmp(args[0], "set") == 0)
	{
		printSET();
	}

	else if (strcmp(args[0], "get") == 0)
	{
		printGET();
	}

	else
	{
		system(args[0]);
	}
}

void buildArgs(char** list){
		fgets(wholeCommand, MAX_NAME_LENGTH , stdin);
		char* eachCommand = strtok(wholeCommand,"\n"); // remove the dangling \n

		while(*eachCommand != '\0'){
			while(*eachCommand == ' ' || *eachCommand == '\n')
				*eachCommand++ = '\0';
				*list++ = eachCommand;
				while(*eachCommand != '\0' && *eachCommand != ' ' && *eachCommand != '\n')
					eachCommand++;
		}
		*list = '\0';
}

void printCD(){
	char *arg;
	if(args[1] != NULL){
		if(strcmp(args[1],"..") == 0)
		{
			arg = "..";
		}
		else if(strcmp(args[1], " ") == 0 ){
			arg = getenv("HOME");
		}
		else {
			getcwd(arg,sizeof(arg));
			printf("arg: %s\n",arg);
			strcat(arg,"/");
			strcat(arg,args[1]);
			printf("args[1]: %s\n",args[1]);
			printf("arg: %s\n",arg);
		}
	} else {
		arg = getenv("HOME");
	}
	if(chdir(arg)){
		printf("Error %s : No such file or directory\n",args[1]);
	}


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
	if(argv[1] != NULL)
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
