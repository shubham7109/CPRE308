#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>

int main(int argc, char** argv)
{
	int fd,choice=0;
    char * myfifo = "myfifo";
	char str[200] = "";
	printf("Enter '1' if you want to read else enter '2' if you want to write: ");
	scanf("%d",&choice);
	/* create the FIFO (named pipe) */
    mkfifo(myfifo, 0666);
    
    while(1){
		if(choice == 1){
		    // Open FIFO for Read only 
		    fd = open(myfifo, O_RDONLY); 
	  
		    // Read from FIFO 
		    read(fd, str, sizeof(str)); 
	  
		    // Print the read message 
		    printf("Output: %s\n", str); 
		    close(fd); 	
		} else if (choice == 2){
			fd = open(myfifo, O_WRONLY);
			fgets(str, 100, stdin);
			write(fd, str, sizeof(strlen(str)));
			close(fd);	
		} else{
			printf("Incorrect choice\n");
			return 0;
		}	
    }
  		return 0;
}

