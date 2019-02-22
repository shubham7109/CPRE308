#include <stdio.h>			//standart library functions
#include <stdlib.h>			//for set&getenv
#include <string.h>			//for string functions
#include <unistd.h>			//for func like fork,chdir etc.
#include <stdbool.h>		//for bool datatype
#include <sys/types.h>	// for waitpid
#include <sys/wait.h>		// for waitpid

#define MAX_NAME_LENGTH 256

char userName[20];
char wholeCommand[MAX_NAME_LENGTH];
char* args[MAX_NAME_LENGTH];
int bgStatus;
int bgPID;
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
void handleUnixCommand(bool isBackground);
bool isBackground();
void checkBg();

int main(int argc, char** argv) {

	//printf("lol\n");
	while(1)
	{
		usleep(10000);
		checkBg();
		printUserName(argv);
		executeCommand();
	}

	return 0;
}

void executeCommand(){
	buildArgs(args);

	// User does not input anything
	if(args[0] == NULL){
		return;
	}

	if (strcmp(args[0], "exit") == 0){
		printf("Terminating the program...\n");
		exit(0);
	}

	else if (strcmp(args[0], "pid") == 0)
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
		if(args[0] != NULL
			&& !strcmp(args[0]," ") == 0
			&& !strcmp(args[0],"\n") == 0
			&& !strcmp(args[0],"\0") == 0
			&& !strcmp(args[0],"\t") == 0){

				handleUnixCommand(isBackground());

		}

	}
}

void handleUnixCommand(bool isBackground){
	int ret = fork();
	int status;
	if (ret == 0) {
		/* this is the child process */
		printf("Child [%d]: Started...\n", getpid());
		execvp(args[0], args);
		printf("Cannot exec %s: No such file or directory\n",args[0]);
		exit(1); // Mannually end the child process

	} else{
		/* this is the parent process */
		if(!isBackground){ // wait only is it's not background process.
			waitpid(ret, &status, NULL);
			if (status){
				printf("ERROR [%d]: %s Exit 1\n", ret, args[0]);
			} else printf("Child [%d], Exit %d \n", ret,status); // Else the process exited normally
		} else {
			bgStatus = status;
			bgPID = ret;
		}
	}
}

void checkBg(){
	int bgPID = waitpid(-1, &bgStatus, WNOHANG);
	if(bgPID>0){
		if(WIFEXITED(bgStatus) != 0)
		{
			if(bgStatus)
				printf("ERROR: Child [%d] Exit 1 (Bg)\n",bgPID);
			else printf("Child [%d] Exit 0 (Bg)\n",bgPID);
		}
		else if(WIFSIGNALED(bgStatus)!=0)
		{
			printf("Child [%d] Killed (Bg)\n",bgPID);
		}
	}
}

void buildArgs(char** list){
	fgets(wholeCommand, MAX_NAME_LENGTH , stdin);
	char* eachCommand = strtok(wholeCommand,"\n"); // remove the dangling \n

	// Command is empty
	if(eachCommand == NULL){
		return;
	}
	while(*eachCommand != '\0'){
		while(*eachCommand == ' ' || *eachCommand == '\n'){
			*eachCommand++ = '\0';
		}
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
		else if(strcmp(args[1], "") == 0 || strcmp(args[1], " ") == 0 ){
			arg = getenv("HOME");
		}
		else {
			char cwd[MAX_NAME_LENGTH];
			getcwd(cwd,sizeof(cwd));
			strcat(cwd,"/");
			strcat(cwd,args[1]);
			arg = cwd;
		}
	} else {
		arg = getenv("HOME");
	}
	printf("%s\n", arg);
	if(chdir(arg)){
		printf("Error %s : No such file or directory\n",args[1]);
	}
}

void printSET(){
	if(args[1] != NULL && args[2] != NULL){
		setenv(args[1],args[2],1);
	} else if(args[1] != NULL && args[2] == NULL){
		unsetenv(args[1]);
	}
	else
		printf("Error: Missing <var> tag\n");
}

void printGET(){
	if(args[1] != NULL)
		printf("Value of %s: %s\n",args[1], getenv(args[1]));
	else
		printf("Error: Missing <var> tag\n");

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
	printf("%s/\n",cwd);
}

bool isBackground(){
	int i=0;
	while(args[i] != NULL){
		i++;
	}
	if(strcmp(args[i-1],"&") == 0){
		args[i-1] = NULL;
		return true;
	}
	return false;
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
