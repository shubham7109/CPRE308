#include <stdio.h>			//standart library functions
#include <stdlib.h>			//for set&getenv
#include <string.h>			//for string functions
#include <unistd.h>			//for func like fork,chdir etc.
#include <stdbool.h>		//for bool datatype
#include <sys/types.h>	// for waitpid
#include <sys/wait.h>		// for waitpid

// Max length of each string
#define MAX_NAME_LENGTH 256

// Saves the name of the user
char userName[20];

// raw command entered by the user
char wholeCommand[MAX_NAME_LENGTH];

// command split into seperate args
char* args[MAX_NAME_LENGTH];

// saves the status of the bg process
int bgStatus;

// saves the pid of the bg process
int bgPID;

// List of all the user defined functions.
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
// End of list of user defined functions.

/*
 * MAIN OF PROGRAM
 * Handles all the arguments to the given specs
 * Will return 0 when 'exit' is typed.
 */
int main(int argc, char** argv) {

	// Infinite loop for the shell
	while(1)
	{
		checkBg();
		printUserName(argv);
		executeCommand();
	}

	return 0;
}

/*
 * Ececutes given built-in-commmands
 * and all linux commands
 * this method is responsible for exiting the
 * program.
 */
void executeCommand(){
	buildArgs(args);

	// User does not input anything
	if(args[0] == NULL ||
		strcmp(args[0]," ") == 0 ||
		strcmp(args[0],"\n") == 0 ||
		strcmp(args[0],"\0") == 0 ||
		strcmp(args[0],"\t") == 0){
		return;
	}

	//Handes if the user wants to exit
	if (strcmp(args[0], "exit") == 0){
		printf("Terminating the program...\n");
		exit(0);
	}

	//Prints the PID of the current process
	else if (strcmp(args[0], "pid") == 0)
	{
		printPID();
	}

	//Prints the parents PID of the current process
	else if (strcmp(args[0], "ppid") == 0)
	{
		printPPID();
	}

	//Prints the current working directory
	else if (strcmp(args[0], "pwd") == 0)
	{
		printPWD();
	}

	//Handles a change in the directory
	else if (strcmp(args[0], "cd") == 0)
	{
		printCD();
	}

	//Sets and resets an environment variable.
	else if (strcmp(args[0], "set") == 0)
	{
		printSET();
	}

	//Gets an environment variable
	else if (strcmp(args[0], "get") == 0)
	{
		printGET();
	}

	//Handles if the user executs a linux command.
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

/*
 * Handles all unix commands.
 * Will check for if proccess exits with
 * or without an error. Also, handles proccess
 * if it is to run in background.
 */
void handleUnixCommand(bool isBackground){
	int ret = fork(); // ret is the pid of the child process.
	int status;
	if (ret == 0) {
		/* this is the child process */
		if(!isBackground)
			printf("Child [%d]: Started...\n", getpid());
		else
			printf("Child [%d]: Started... (Bg)\n", getpid());

		execvp(args[0], args); // Executes the command.

		printf("Cannot exec %s: No such file or directory\n",args[0]); // Runs when execvp fails.

		exit(1); // Mannually end the child process

	} else{
		/* this is the parent process */
		if(!isBackground){ // wait only is it's not background process.
			waitpid(ret, &status, NULL);
			if (status){
				printf("ERROR [%d]: %s Exit 1\n", ret, args[0]);
			} else printf("Child [%d], Exit %d \n", ret,status); // Else the process exited normally
		} else {
			// Set global variables since its a bg procss.
			bgStatus = status;
			bgPID = ret;
		}
	}
}

/*
 * Handles the termination of a bg process.
 * Bg process can end with or without an error,
 * which is handled accordingly.
 */
void checkBg(){
	usleep(100000); // Needed for the printf statement to run from main();
	int bgPID = waitpid(-1, &bgStatus, WNOHANG);

	if(bgPID>0){ //If there is a bg process currently running.
		if(WIFEXITED(bgStatus) != 0)
		{
			if(bgStatus)
				printf("ERROR: Child [%d] Exit 1 (Bg)\n",bgPID);
			else printf("Child [%d], Exit 0 (Bg)\n",bgPID);
		}
		else if(WIFSIGNALED(bgStatus)!=0)
		{
			printf("Child [%d], Killed (Bg)\n",bgPID); //The bg process for forcefully killed
		}
	}
}

/*
 * Builds the args needed for execvp command.
 * Splits the while command to usable sections in
 * to a char array[]
 */
void buildArgs(char** arguments){
	fgets(wholeCommand, MAX_NAME_LENGTH , stdin); // Reads the whole line at once.
	char* eachCommand = strtok(wholeCommand,"\n"); // remove the dangling \n

	// Command is empty
	if(eachCommand == NULL){
		return;
	}

	// Builds each arg[]
	while(*eachCommand != '\0'){
		while(*eachCommand == ' ' || *eachCommand == '\n'){
			*eachCommand++ = '\0';
		}
			*arguments++ = eachCommand;
			while(*eachCommand != '\0' && *eachCommand != ' ' && *eachCommand != '\n')
				eachCommand++;
	}
	*arguments = '\0';
}

/*
 * Handles and prints the cd command.
 * cd moves pwd to HOME
 * and it is able to move from its relative path.
 */
void printCD(){
	char *arg;
	if(args[1] != NULL){
		//If the user wants to move back a dir
		if(strcmp(args[1],"..") == 0)
		{
			arg = "..";
		}
		// Else if cd is empty
		else if(strcmp(args[1], "") == 0 || strcmp(args[1], " ") == 0 ){
			arg = getenv("HOME"); // Home dir of the user.
		}
		// Else just cd to the given folder.
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
	//executes the cd command appropriately.
	if(chdir(arg)){
		// Reaches here if there is an error.
		printf("Error %s : No such file or directory\n",args[1]);
	}
}

/*
 * Sets and resets am environment var
 */
void printSET(){
	if(args[1] != NULL && args[2] != NULL){
		setenv(args[1],args[2],1);
	} else if(args[1] != NULL && args[2] == NULL){
		unsetenv(args[1]);
	}
	else
		printf("Error: Missing <var> tag\n");
}

/*
 * Prints the given environment var
 */
void printGET(){
	if(args[1] != NULL)
		printf("Value of %s: %s\n",args[1], getenv(args[1]));
	else
		printf("Error: Missing <var> tag\n");

}

/*
 * Prints the current pid
 */
void printPID(){
	printf("PID: %d\n",getpid());
}

/*
 * Prints the PPID
 */
void printPPID(){
	printf("PPID: %d\n",getppid());
}

/*
 * Prints the current working dir
 */
void printPWD(){
	char cwd[MAX_NAME_LENGTH];
	getcwd(cwd,sizeof(cwd));
	printf("%s/\n",cwd);
}

/*
 * Returns true if the user wants to
 * run the command in the bg.
 * else retruns false.
 */
bool isBackground(){
	int i=0;
	while(args[i] != NULL){
		i++; // Counts the index of the last argument.
	}

	//Checks if the last argument is '&'
	if(strcmp(args[i-1],"&") == 0){
		args[i-1] = NULL;
		return true;
	}
	return false;
}

/*
 * Prints the username of the shell
 * Handles a custom prompt '-p'
 */
void printUserName(char **argv){
	strcpy(userName, "308sh> ");
	if(argv[1] != NULL)
		if(strcmp(argv[1], "-p") == 0){
			strcpy(userName, argv[2]);

		}

	int i=0;
	for(i = 0; i <= strlen(userName); i++)
  	{
  		if(userName[i] == '\"')
		{
  			userName[i] = '\0';
 		}
	}
	printf("%s",userName);
}
