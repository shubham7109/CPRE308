CC = gcc
CFLAGS  = -g -Wall

default: lab

lab:   lab1.o message.o
	$(CC) $(CFLAGS) -o lab1 lab1.o message.o

lab1.o:  lab1.c message.h 
	$(CC) $(CFLAGS) -c lab1.c

counter.o:  message.c message.h 
	$(CC) $(CFLAGS) -c message.c

clean: 
	$(RM) lab1 *.o *~

