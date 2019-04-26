#include <stdio.h>
#include <sys/stat.h> 
#include <fcntl.h>

int main()
{

    char *buf1 = (char *) calloc(100, sizeof(char)); 
    char *buf2 = (char *) calloc(100, sizeof(char)); 
    char *buf3 = "abcde"; 
    char *buf4 = (char *) calloc(100, sizeof(char)); 
    char *buf5 = (char *) calloc(100, sizeof(char)); 
    int fdr, fdrw, fdrw2;

    fdrw = open("file", O_RDONLY);

    int pid = fork();

    if(pid == 0){
        read(fdrw, buf1, 3);
        printf("Parent: %s\n",buf1);
    }else if(pid > 0){
        read(fdrw, buf2, 4);
        printf("Child: %s\n",buf2);
    }

   return 0;
}